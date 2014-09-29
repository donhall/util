#include "Worker.h"
#include <Windows.h>
#include <iostream>

using namespace std;

CWorker::CWorker(int nWorkerId, boost::thread_group &group)
	:m_nWorkerId(nWorkerId), m_group(group)
{
	start();
	m_bLoaded = false;
}


CWorker::~CWorker(void)
{
}

bool CWorker::isLoaded()
{
	boost::mutex::scoped_lock lock(m_mutex); 
	return m_bLoaded;
};

bool CWorker::tryLoad(CRecords *ptrRecord)
{
	boost::mutex::scoped_lock lock(m_mutex);
	m_ptrRecord = ptrRecord;
	m_bLoaded = true;
	return true;
}

bool CWorker::start()
{
	//TODO:create thread
	m_group.create_thread(boost::bind(&CWorker::workerProcess, this));

	return true;
}

void CWorker::workerProcess()
{
	while (1)
	{
		if (!isLoaded())
		{
			Sleep(WAIT_DURATION);
			continue;
		}

//		for (int i=0; i<20; i++)
		{
			cout<<"thread "<<m_nWorkerId<<" Record id="<<m_ptrRecord->getId()<<" msg="<<m_ptrRecord->getMsg()<<endl;
			Sleep(500);
			stringstream sst;
			sst<<m_nWorkerId;
			m_ptrRecord->setMsg(sst.str());
		}

		m_bLoaded = false;
	}
}
