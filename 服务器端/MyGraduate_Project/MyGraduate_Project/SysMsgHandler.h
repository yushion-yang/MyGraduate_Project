#ifndef __SYS_MSG_HANDLER_H__
#define __SYS_MSG_HANDLER_H__
class ClientState;
class BaseMsg;
class SysMsgHandler
{
public:
	SysMsgHandler();
	~SysMsgHandler();
	void RegisterFun();
	void MsgPing(ClientState* client, BaseMsg * msg);
private:

};
#endif // !__SYS_MSG_HANDLER_H__
