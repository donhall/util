#include "ThreadManager.h"
#include <Windows.h>

using namespace std;
CThreadManager::CThreadManager(void):m_bInited(false)
{

}


CThreadManager::~CThreadManager(void)
{
	for (vector<CWorker*>::iterator it = m_vAllThreads.begin(); it != m_vAllThreads.end(); it++)
	{
		delete *it;
	}
}

int CThreadManager::initPool(int number)
{
	int succ=0;
	if (m_bInited)
	{
		return 0;
	}

	boost::mutex::scoped_lock lock(m_mutex);
	for (int i=0; i<number; i++)
	{
		CWorker *worker = new CWorker(i, m_poolGroup);
		m_vAllThreads.push_back(worker);
		m_sIdleThreadId.insert(i);
		succ ++;
	}
	lock.unlock();

	m_scanGroup.create_thread(boost::bind(&CThreadManager::poolControl, this));
	m_scanGroup.create_thread(boost::bind(&CThreadManager::queueControl, this));

	m_bInited = true;
	m_bFinished = true;
	return succ;
}

int CThreadManager::addTask(CRecords *record)
{
	while (m_qTasks.insertTask(record) == -1)
	{
		Sleep(WAIT_DURATION);
	}
	m_bFinished = false;
	return 0;
}

void CThreadManager::toFinish()
{
	while (1)
	{
		if (m_qTasks.isEmpty())
		{
			boost::mutex::scoped_lock lock(m_mutex);
			if (m_sBusyThreadId.empty())
				return;
		}

		Sleep(WAIT_DURATION);
	}
}

void CThreadManager::poolControl()
{
	while (1)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		for (vector<CWorker*>::iterator it = m_vAllThreads.begin(); it != m_vAllThreads.end(); it++)
		{
			if ((*it)->isLoaded())
			{
				int t_nId = (*it)->getWorkerId();
				set<int>::iterator sit = m_sIdleThreadId.find(t_nId);
				if (sit != m_sIdleThreadId.end())
				{
					m_sIdleThreadId.erase(sit);
				}

				sit = m_sBusyThreadId.find(t_nId);
				if (sit == m_sBusyThreadId.end())
				{
					m_sBusyThreadId.insert(t_nId);
				}
			} else
			{
				int t_nId = (*it)->getWorkerId();
				set<int>::iterator sit = m_sIdleThreadId.find(t_nId);
				if (sit == m_sIdleThreadId.end())
				{
					m_sIdleThreadId.insert(t_nId);
				}

				sit = m_sBusyThreadId.find(t_nId);
				if (sit != m_sBusyThreadId.end())
				{
					m_sBusyThreadId.erase(sit);
				}
			}
		}
		lock.unlock();

		Sleep(WAIT_DURATION);
	}
}

void CThreadManager::queueControl()
{
	CRecords *record;

	while(1)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		while (m_qTasks.popupTask(&record) >= 0)
		{
			while (m_sIdleThreadId.empty())
			{
				lock.unlock();
				Sleep(WAIT_DURATION);
				lock.lock();
			}
			set<int>::iterator sit = m_sIdleThreadId.begin();
			int t_nId = *sit;
			CWorker *w = m_vAllThreads[t_nId];
			w->tryLoad(record);

			m_sIdleThreadId.erase(t_nId);
			m_sBusyThreadId.insert(t_nId);
		}
		lock.unlock();
		Sleep(WAIT_DURATION);
	}
}