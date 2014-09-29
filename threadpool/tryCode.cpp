#include <ctime>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include "ThreadManager.h"
#include "Records.h"

//#include "RsaDecrypt.h"

#define RNUM	20

using namespace std;

int poolTest()
{
	time_t begin, end;
	char chsVal[9]={0};
	vector<CRecords *> llRec;
	srand(time(NULL));

	for (int i=100; i<100+RNUM; i++)
	{
		for (int j=0; j<8; j++)
		{
			chsVal[j]=rand()%26 + 97;
		}
		CRecords *rec = new CRecords(i, chsVal);
		llRec.push_back(rec);
	}

	CThreadManager mng;
	mng.initPool(10);

	begin = clock();

	for(int i=0; i<RNUM; i++)
	{
		mng.addTask(llRec.at(i));
	}

	mng.toFinish();

	for(int i=0; i<RNUM; i++)
	{
		cout<<"rec.id="<<llRec[i]->getId()<<", rec.msg="<<llRec[i]->getMsg()<<endl;
		delete llRec[i];
	}

	end = clock();

	cout<<"RUNNING TIME:"<<end-begin<<"ms"<<endl;

	while (1)
	{
		cout<<"still running..."<<endl;
		Sleep(3000);
	}

}

int containerTest()
{
	list<CRecords> llr;
	//undefined behavior
	CRecords rr = llr.front();

	cout<<"id="<<rr.getId()<<", msg="<<rr.getMsg()<<endl;
	return 0;
}

int main()
{
	cout<<"start running..."<<endl;
	poolTest();

	//containerTest();
}
