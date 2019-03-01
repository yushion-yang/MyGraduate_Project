#include "SysMsgHandler.h"
#include "Reflection.h"
#include "Singleton.h"
#include "ClientState.h"
#include "NetManager.h"

SysMsgHandler::SysMsgHandler()
{

}

SysMsgHandler::~SysMsgHandler()
{

}

void SysMsgHandler::RegisterFun()
{
	REGISTE_FUN(SysMsgHandler, MsgPing);
}

void SysMsgHandler::MsgPing(ClientState * client, BaseMsg * msg)
{
	//TODO 收到ping 刷新时间并回复Pong
	printf("Ping\n");
	//client->SetPingTime();

	//c.lastPingTime = NetManager.GetTimeStamp();
	//MsgPong msgPong = new MsgPong();
	//NetManager.Send(c, msgPong);
}
