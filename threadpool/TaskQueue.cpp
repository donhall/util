#include "TaskQueue.h"


CTaskQueue::CTaskQueue(void)
{
}


CTaskQueue::~CTaskQueue(void)
{
}

//return -1 if Q is full,
//otherwise return Q size.
int CTaskQueue::insertTask(CRecords *ptrRecord)
{
	boost::mutex::scoped_lock lock(m_mutex);
	//Queue is full?
	if(m_vQs.size() >=  MAX_QUEUE_SIZE)
		return -1;
	
	m_vQs.push(ptrRecord);
	return m_vQs.size();
};

//return -1 if Q is empty,
//otherwise return Q's size.
int CTaskQueue::popupTask(CRecords **ptrRecord)
{
	boost::mutex::scoped_lock lock(m_mutex);

	if (m_vQs.empty())
		return -1;

	*ptrRecord = m_vQs.front();
	m_vQs.pop();

	return m_vQs.size();
}