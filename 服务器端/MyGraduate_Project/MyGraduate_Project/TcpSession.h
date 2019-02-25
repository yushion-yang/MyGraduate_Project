#ifndef __TCP_SESSION_H__
#define __TCP_SESSION_H__

#include "TcpConnection.h"

#include <memory>

class TcpSession
{
public:
	TcpSession();
	TcpSession(const std::weak_ptr<TcpConnection>& tmpconn);
	~TcpSession();

	TcpSession(const TcpSession& rhs) = delete;
	TcpSession& operator =(const TcpSession& rhs) = delete;

	std::shared_ptr<TcpConnection> GetConnectionPtr()
	{
		if (tmpConn_.expired())
			return NULL;

		return tmpConn_.lock();
	}

	void Send(int32_t cmd, int32_t seq, const std::string& data);
	void Send(int32_t cmd, int32_t seq, const char* data, int32_t dataLength);
	void Send(const std::string& p);
	void Send(const char* p, int32_t length);

private:
	void SendPackage(const char* p, int32_t length);

protected:
	//TcpSession引用TcpConnection类必须是弱指针，因为TcpConnection可能会因网络出错自己销毁，此时TcpSession应该也要销毁
	std::weak_ptr<TcpConnection>    tmpConn_;
	//std::shared_ptr<TcpConnection>    tmpConn_;
};

#endif // !__TCP_SESSION_H__
