#ifndef __NET_MANAGER_H__
#define __NET_MANAGER_H__
#include <map>

using namespace std;

class ClientState;
class TcpConnection;
class Buffer;
class BaseMsg;
class NetManager
{
	using ClientStates = std::map<int, ClientState*>;
public:
	NetManager();
	~NetManager();
	void RegisterFun();
	void Close(ClientState* state);
	void SolveData(TcpConnection* connection, Buffer* buffer);
	void Send(ClientState* cs, BaseMsg* msg);
	void CheckPing();
	void AddClient(TcpConnection* conn);
	int64_t TimeNow();
private:
	ClientStates _clientStates;
};
#endif // !__NET_MANAGER_H__
