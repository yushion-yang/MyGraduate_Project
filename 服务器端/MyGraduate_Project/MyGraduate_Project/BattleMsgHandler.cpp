#include "BattleMsgHandler.h"
#include "Reflection.h"
#include "Singleton.h"
#include "ClientState.h"
#include "NetManager.h"

BattleMsgHandler::BattleMsgHandler()
{

}

BattleMsgHandler::~BattleMsgHandler()
{

}

void BattleMsgHandler::RegisterFun()
{
	REGISTE_FUN(BattleMsgHandler,MsgCreateTurret);
}

void BattleMsgHandler::MsgCreateTurret(ClientState * client, BaseMsg * msg)
{
	printf("MsgCreateTurret\n");
	Singleton<NetManager>::Instance().Send(client, msg);
}
