#include "MsgCreateTurret.h"

MsgCreateTurret::MsgCreateTurret()
{

}


MsgCreateTurret::~MsgCreateTurret()
{

}

char * MsgCreateTurret::EnCode()
{
	SET_VALUE(_protoName);
	SET_VALUE(_x);
	SET_VALUE(_y);
	SET_VALUE(_z);
	SET_VALUE(_NO);
	GET_CHARS_RETURN;
}

BaseMsg * MsgCreateTurret::DeCode(Json::Value value)
{
	GET_VALUE(_protoName, String);
	GET_VALUE(_x, Double);
	GET_VALUE(_y, Double);
	GET_VALUE(_z, Double);
	GET_VALUE(_NO, Int);
	return this;
}

void MsgCreateTurret::PrintData()
{
	std::cout << "_protoName" << _protoName << " _x:" << _x << " _y:"
		<< _y << " _z:" << _z << " _NO:" << _NO << std::endl;
}
