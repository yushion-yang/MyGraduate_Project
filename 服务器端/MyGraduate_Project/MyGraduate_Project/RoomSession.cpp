#include "RoomSession.h"

RoomSession::RoomSession()
{

}

RoomSession::RoomSession(const std::shared_ptr<TcpConnection>& conn, int sessionid) :
	TcpSession(conn),
	m_id_(sessionid)
{
	m_userinfo_.userid = 0;
}

RoomSession::~RoomSession()
{

}

Player* RoomSession::AddPlayer(TcpConnection * conn)
{
	if (player_count_ > 3)
	{
		return nullptr;
	}
	//player_[player_count_++].SetTcpConnection(conn);
	return &player_[player_count_ - 1];
}

bool RoomSession::RsEnoughPlayer()
{
	return player_count_ == 4;;
}

void RoomSession::OnRead(TcpConnection* conn, Buffer * pBuffer)
{

}
