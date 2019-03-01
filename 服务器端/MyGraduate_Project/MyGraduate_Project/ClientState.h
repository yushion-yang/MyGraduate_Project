#ifndef __CLIENT_STATE_H__
#define __CLIENT_STATE_H__
#include <stdint.h>
class Player;
class TcpConnection;

class ClientState
{
public:
	ClientState(TcpConnection *connection);
	~ClientState();
	void Close();
	void SetPlayer(Player* player);
	int socket() { return _socket; }
	void Send(char *datas, int dataLen);
	bool CheckTime(int64_t timeNow);
private:
	Player* _player;
	int64_t _lastPingTime;
	TcpConnection* _connection;
	int _socket;
};
#endif // !__CLIENT_STATE_H__
