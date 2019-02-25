#include "TcpServer.h"
#include "MyThreadPool.h"
#include "EventLoop.h"
#include "Logger.h"
#include "Global.h"
#include "Singleton.h"

#include <assert.h>

TcpServer::TcpServer(EventLoop* loop):started_(false),nextConnId_(1)
{
	assert(loop);
	loop_ = loop;
	acceptor_.reset(new Acceptor(loop_));
	acceptor_->SetNewConnectionCallBack(std::bind(&TcpServer::newConnection,this, std::placeholders::_1));
}

TcpServer::~TcpServer()
{
	loop_->assertInLoopThread();
	LogInfo ( "TcpServer::~TcpServer destructing");

	//移除所有连接
	for (TcpConnctions::iterator it(tcp_connections_.begin());it != tcp_connections_.end(); ++it)
	{
		TcpConnectionPtr conn = *it;
		(*it).reset();
		conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
		conn.reset();
	}
}

void TcpServer::Start()
{
	if (started_ == 0)
	{
		//threadPool_->start(threadInitCallback_);
		//assert(!acceptor_->listenning());
		////设置监听任务
		loop_->runInLoop(std::bind(&Acceptor::Listen, acceptor_.get()));
		started_ = 1;
	}
}

void TcpServer::newConnection(int socket)
{
	EventLoop* ioLoop = Singleton<MyThreadPool>::Instance().GetNextLoop();
	TcpConnectionPtr new_connection(new TcpConnection(socket,ioLoop));
	//TODO	移除player和connection的直接连接关系
	new_connection->SetPlayer(lobby_.GetRoomSession());

	tcp_connections_.push_back(new_connection);
	new_connection->setConnectionCallback(connectionCallback_);	//IMServer::OnConnection
	new_connection->setMessageCallback(messageCallback_);
	new_connection->setWriteCompleteCallback(writeCompleteCallback_);	//没有对应事件  为空
	new_connection->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1)); // FIXME: unsafe
	//该线程分离完io事件后，立即调用TcpConnection::connectEstablished	-->IMServer::OnConnection
	ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, new_connection));
}

void TcpServer::removeConnection(const TcpConnectionPtr & conn)
{
	loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr & conn)
{
	loop_->assertInLoopThread();
	LogInfo("TcpServer::removeConnectionInLoop one connection");
	bool flag = false;
	for (auto tconn = tcp_connections_.begin(); tconn != tcp_connections_.end(); ++tconn)
	{
		if (*tconn == conn)
		{
			//只是从集合中删除  还没有释放
			tcp_connections_.erase(tconn);
			flag = true;
			break;
		}
	}
	if (!flag)
	{
		//出现这种情况，是TcpConneaction对象在创建过程中，对方就断开连接了。
		LogInfo("want to remove a connection ,but connection does not exist.");
		return;
	}
	EventLoop* ioLoop = conn->getLoop();
	//必然在别的线程执行
	ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}
