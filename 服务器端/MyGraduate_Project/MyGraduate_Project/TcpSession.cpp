#include "TcpSession.h"

TcpSession::TcpSession()
{
}

TcpSession::TcpSession(const std::weak_ptr<TcpConnection>& tmpconn) : tmpConn_(tmpconn)
{

}

TcpSession::~TcpSession()
{

}

void TcpSession::Send(int32_t cmd, int32_t seq, const std::string& data)
{
	Send(cmd, seq, data.c_str(), data.length());
}

void TcpSession::Send(int32_t cmd, int32_t seq, const char* data, int32_t dataLength)
{
	std::string outbuf;

	SendPackage(outbuf.c_str(), outbuf.length());
}

void TcpSession::Send(const std::string& p)
{
	SendPackage(p.c_str(), p.length());
}

void TcpSession::Send(const char* p, int32_t length)
{
	SendPackage(p, length);
}

void TcpSession::SendPackage(const char* p, int32_t length)
{

}
