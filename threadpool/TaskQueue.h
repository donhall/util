#ifndef TASKQUQUE_H_
#define TASKQUQUE_H_

#include <string>
#include <queue>
#include <boost/thread.hpp>
#include "Records.h"

#define MAX_QUEUE_SIZE	50

class CTaskQueue
{
public:
	CTaskQueue(void);
	~CTaskQueue(void);

	int insertTask(CRecords *ptrRecord);
	int popupTask(CRecords** ptrRecord);
	int getQueueSize(){boost::mutex::scoped_lock lock(m_mutex); return m_vQs.size();};
	bool isEmpty(){boost::mutex::scoped_lock lock(m_mutex); return m_vQs.empty();};

private:
	boost::mutex m_mutex;
	std::queue <CRecords *> m_vQs;

};

#endif