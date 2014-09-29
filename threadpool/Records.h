#ifndef RECORDS_H_
#define RECORDS_H_

#include <string>

#define WAIT_DURATION	  10

class CRecords
{
public:
	CRecords(void){};
	CRecords(int nId, std::string strMsg):m_nId(nId),m_strMsg(strMsg){};
	~CRecords(void){};

	std::string getMsg(){return m_strMsg;};
	int getId(){return m_nId;};

	void setMsg(std::string strMsg){m_strMsg = strMsg;};
	void setId(int nId){m_nId = nId;};
private:
	int m_nId;
	std::string m_strMsg;
};

#endif
