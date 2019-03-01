#ifndef __BATTLE_MSG_HANDLER_H__
#define __BATTLE_MSG_HANDLER_H__

class ClientState;
class BaseMsg;

class BattleMsgHandler
{
public:
	BattleMsgHandler();
	~BattleMsgHandler();
	void RegisterFun();
	void MsgCreateTurret(ClientState* client ,BaseMsg * msg);
private:

};
#endif // !__BATTLE_MSG_HANDLER_H__
