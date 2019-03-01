#include "ClientState.h"
#include "TcpConnection.h"
#include "Singleton.h"
#include "NetManager.h"

ClientState::ClientState(TcpConnection * connection)
{
	_connection = connection;
	_socket = connection->socket_;
	_lastPingTime = Singleton<NetManager>::Instance().TimeNow();
}

ClientState::~ClientState()
{

}

void ClientState::Close()
{
	if (_connection != NULL)
	{
		_connection->SolveClose();
	}
}

void ClientState::SetPlayer(Player * player)
{
	_player = player;
}

void ClientState::Send(char * datas, int dataLen)
{
	_connection->send(datas, dataLen);
}

bool ClientState::CheckTime(int64_t timeNow)
{
	if (timeNow - _lastPingTime > 120) {
		return true;
	}
	return false;
}
