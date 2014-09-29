#ifndef WORKER_H_
#define WORKER_H_

#include "Records.h"
#include <boost/thread.hpp>

class CWorker
{
public:
	CWorker(int nWorkerId, boost::thread_group &group);
	~CWorker(void);

	bool isLoaded();
	bool tryLoad(CRecords *ptrRecord);

	int getWorkerId(){return m_nWorkerId;};
private:
	int m_nWorkerId;

	boost::thread_group &m_group;

	boost::mutex m_mutex;
	bool m_bLoaded;
	CRecords *m_ptrRecord;
	void (*report)(int);

	bool start();
	void workerProcess();
};

#endif
