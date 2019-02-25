#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "Acceptor.h"
#include "TcpConnection.h"
#include "Lobby.h"

#include <memory>
#include <vector>

class EventLoop;

class TcpServer
{
	using TcpConnctions = std::vector<std::shared_ptr<TcpConnection> >;
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
	typedef std::function<void(TcpConnection*, Buffer*)> MessageCallback;
	typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;
public:
	TcpServer(EventLoop* loop);
	~TcpServer();

	EventLoop* getLoop() const { return loop_; }
	void Start();

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}
	void setMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}
	void setWriteCompleteCallback(const WriteCompleteCallback& cb)
	{
		writeCompleteCallback_ = cb;
	}

	void removeConnection(const TcpConnectionPtr& conn);
private:
	/// Not thread safe, but in loop
	void newConnection(int sockfd);
	/// Thread safe.

	/// Not thread safe, but in loop
	void removeConnectionInLoop(const TcpConnectionPtr& conn);
private:
	std::shared_ptr<Acceptor> acceptor_;
	EventLoop* loop_;
	TcpConnctions tcp_connections_;
	Lobby lobby_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	std::atomic<int> started_;
	int nextConnId_;  // always in loop thread
};

#endif // !__TCP_SERVER_H__
