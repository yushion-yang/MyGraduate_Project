#ifndef __MSG_CREATE_TURRET_H__
#define __MSG_CREATE_TURRET_H__

#include "BaseMsg.h"

class MsgCreateTurret :	public BaseMsg
{
public:
	double _x;
	double _y;
	double _z;
	int _NO;
public:
	MsgCreateTurret();
	virtual ~MsgCreateTurret();
	char* EnCode(int &dataLen);
	BaseMsg* DeCode(Json::Value value);
	void PrintData();
};
#endif // !__MSG_CREATE_TURRET_H__
