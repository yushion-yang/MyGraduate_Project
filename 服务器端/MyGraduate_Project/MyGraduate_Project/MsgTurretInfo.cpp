#include "MsgTurretInfo.h"


MsgTurretInfo::MsgTurretInfo()
{

}

void MsgTurretInfo::Init()
{
	_protoName = "MsgTurretInfo";
	_turretName = "TankTurret";
	_lv = 10;
	_attackDistence = 2.34234;
}

MsgTurretInfo::~MsgTurretInfo()
{

}

char * MsgTurretInfo::EnCode(int &dataLen)
{
	SET_VALUE(_protoName);
	SET_VALUE(_turretName);
	SET_VALUE(_lv);
	SET_VALUE(_attackDistence);
	GET_CHARS_RETURN;
}

BaseMsg * MsgTurretInfo::DeCode(Json::Value value)
{
	GET_VALUE(_protoName, String);
	GET_VALUE(_turretName, String);
	GET_VALUE(_lv, Int);
	GET_VALUE(_attackDistence, Double);
	return this;
}

void MsgTurretInfo::PrintData()
{
	std::cout << "protoNmae:" << _protoName << " turretName:" << _turretName << " lv:" << _lv
		<< " attackDistance:" << _attackDistence << std::endl;
}