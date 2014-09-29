#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_

#include <set>
#include <vector>
#include <boost/thread.hpp>
#include "Worker.h"
#include "TaskQueue.h"
#include "Records.h"

class CThreadManager
{
public:
	CThreadManager(void);
	~CThreadManager(void);

	int initPool(int number);
	int addTask(CRecords *record);

	int getIdleNumber(){boost::mutex::scoped_lock lock(m_mutex); return m_sIdleThreadId.size();};
	int getBusyNumber(){boost::mutex::scoped_lock lock(m_mutex); return m_sBusyThreadId.size();};

	void toFinish();
private:
	bool m_bInited;
	bool m_bFinished;
	boost::thread_group m_poolGroup;
	boost::thread_group m_scanGroup;

	CTaskQueue m_qTasks;

	std::vector<CWorker*> m_vAllThreads;

	boost::mutex m_mutex;
	std::set<int> m_sIdleThreadId;
	std::set<int> m_sBusyThreadId;

	void poolControl();
	void queueControl();
};

#endif
