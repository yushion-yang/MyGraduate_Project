#ifndef __MSG_TURRET_INFO_H__
#define __MSG_TURRET_INFO_H__

#include "BaseMsg.h"

class MsgTurretInfo :public BaseMsg
{
public:
	std::string _turretName;
	int _lv;
	double _attackDistence;
public:
	void PrintData();
	void Init();
	MsgTurretInfo();
	virtual ~MsgTurretInfo();
	char* EnCode(int &dataLen);
	BaseMsg* DeCode(Json::Value);
};
#endif // !__MSG_TURRET_INFO_H__