#ifndef __MSG_PING_H__
#define _MSG_PING_H__

#include "BaseMsg.h"

class MsgPing:public BaseMsg
{
public:
	MsgPing();
	~MsgPing();
	char* EnCode(int &dataLen);
	BaseMsg* DeCode(Json::Value value);
	void PrintData();
private:

};
#endif // !__MSG_PING_H__
