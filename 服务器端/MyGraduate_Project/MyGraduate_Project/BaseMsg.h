#ifndef __BASE_MSG_H__
#define __BASE_MSG_H__
#include "json/json.h"
#include <cstring>

#pragma warning( disable:4996)

#define SET_VALUE(x) (root[#x] = x)
#define GET_VALUE(x,type) x = value[#x].as##type()
#define GET_INT16(x) (((int16_t*)(x))[0])

#define GET_CHARS_RETURN 	Json::FastWriter writer;\
	std::string strWrite = writer.write(root);\
	char *datas = new char[strWrite.length()];\
	strcpy(datas, strWrite.c_str());\
	return datas;

class BaseMsg
{
public:
	std::string _protoName;
public:
	BaseMsg();
	virtual ~BaseMsg();
	virtual void PrintData();
	virtual void Init();

	virtual char* EnCode();
	virtual BaseMsg* DeCode(Json::Value);

	static BaseMsg* DeCode(char* datas);
	static BaseMsg* GetMsg(const char* protoName);
protected:
	Json::Value root;
};
#endif __BASE_MSG_H__
/*
例子
	//MsgBase msgBase = MsgBase.Decode(protoName, readBuff.bytes
	//获取一个json数据用于测试
	BaseMsg * msgCreateTurret = new MsgTurretInfo();
	msgCreateTurret->Init();
	msgCreateTurret->PrintData();
	char * msgDatas = msgCreateTurret->EnCode();

	BaseMsg* newMsgTurretInfo = BaseMsg::DeCode(msgDatas);
	newMsgTurretInfo->PrintData();
*/