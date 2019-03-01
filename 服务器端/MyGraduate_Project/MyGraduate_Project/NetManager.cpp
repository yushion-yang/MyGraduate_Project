#include "NetManager.h"
#include "Reflection.h"
#include "ClientState.h"
#include "Buffer.h"
#include "BaseMsg.h"
#include "TcpConnection.h"
#include "Singleton.h"
#include "EventHandler.h"
#include "Logger.h"
#include "BattleMsgHandler.h"
#include "SysMsgHandler.h"
#include <chrono>

#define GET_INT16(x) (((int16_t*)(x))[0])
#define REGISTE_CLASS_FUN(className) Singleton<className>::Instance().RegisterFun()

NetManager::NetManager()
{

}

NetManager::~NetManager()
{

}

void NetManager::RegisterFun()
{
	REGISTE_CLASS_FUN(BattleMsgHandler);
	REGISTE_CLASS_FUN(SysMsgHandler);
}

void NetManager::Close(ClientState * state)
{
	LogInfo("close client ,socket:%d", state->socket());
	Singleton<EventHandler>::Instance().OnDisconnect(state);
	state->Close();
	std::map<int, ClientState*>::iterator key = _clientStates.find(state->socket());
	if (key != _clientStates.end())
	{
		_clientStates.erase(key);
	}
	else
	{
		LogError("can not find client in clients map key is:%d", state->socket());
	}
}

void NetManager::SolveData(TcpConnection* connection, Buffer* buffer)
{
	LogInfo("NetManager solveData");
	while (buffer->RsEnoughHeadLen())
	{
		buffer->SetSolveDataLen();
		if (!buffer->RsEnoughOnePackageLen())
		{
			return;
		}
		char *datas = buffer->GetDatas();
		//{"_protoName":"MsgCreateTurret","_x":50.0,"_y":0.0,"_z":50.0,"_NO":0}
		//{"_NO":0,"_protoName":"MsgCreateTurret","_x":50.0,"_y":0.0,"_z":50.0}
		BaseMsg* baseMsg = BaseMsg::DeCode(datas);
		//TODO 存在一个问题 Msg编码之后和原字符串相比多出一个字节 
		//对字符串进行Json反序列化  然后再序列化成字符串  可能会改变字符串的长度吗？  
		//{
		//	int bodyDataLen;
		//	char *tempData = baseMsg->EnCode(bodyDataLen);

		//	int protoNameLen = baseMsg->_protoName.length();
		//	if (0 == strcmp(tempData, datas + 4 + protoNameLen))
		//	{
		//		printf("right \n");
		//	}
		//	//return;
		//}

		INVOKE_FUN(baseMsg->_protoName, _clientStates[connection->socket_], baseMsg);
		buffer->SolveLen();
	}
}

void NetManager::Send(ClientState * cs, BaseMsg * msg)
{
	if (cs == NULL) 
	{
		return;
	}
	if (msg == NULL)
	{
		return;
	}
	LogInfo("Logic Send data");
	int bodyDataLen;
	char datas[1024];
	char *tempData = msg->EnCode(bodyDataLen);
	
	int protoNameLen = msg->_protoName.length();
	memcpy(datas + 4, msg->_protoName.c_str(), protoNameLen);
	memcpy(datas + 4 + protoNameLen, tempData, bodyDataLen);
	int allDataLen = 4 + protoNameLen + bodyDataLen;
	GET_INT16(datas) = allDataLen-2;
	GET_INT16(datas + 2) = protoNameLen;

	BaseMsg* baseMsg = BaseMsg::DeCode(datas);
	cs->Send(datas,allDataLen);
}

void NetManager::CheckPing()
{
//	LogInfo("checking ping");
	int64_t timeNow = TimeNow();
	std::map<int, ClientState*>::iterator it;
	it = _clientStates.begin();
	while (it != _clientStates.end())
	{
		if (it->second->CheckTime(timeNow))
		{
			Close(it->second);
			continue;
		}
		it++;
	}
}

void NetManager::AddClient(TcpConnection * conn)
{
	int key = conn->socket_;
	LogInfo("add new client key:%d", key);
	_clientStates[key] = new ClientState(conn);
}

int64_t NetManager::TimeNow()
{
	chrono::time_point<chrono::system_clock, chrono::microseconds> now =
		chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now());
	return now.time_since_epoch().count();
}
