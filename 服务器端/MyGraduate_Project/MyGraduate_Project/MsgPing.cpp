#include "MsgPing.h"

MsgPing::MsgPing()
{
}

MsgPing::~MsgPing()
{
}

char * MsgPing::EnCode(int & dataLen)
{
	SET_VALUE(_protoName);
	GET_CHARS_RETURN;
}

BaseMsg * MsgPing::DeCode(Json::Value value)
{
	GET_VALUE(_protoName, String);
	return this;
}

void MsgPing::PrintData()
{

}
