#include "TcpConnection.h"
#include "Global.h"
#include "Player.h"
#include "Logger.h"
#include "RoomSession.h"
#include "EventLoop.h"
#include "MsgCreateTurret.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <assert.h>
#include <netinet/tcp.h>
#include <netinet/in.h>


TcpConnection::TcpConnection(int socket,EventLoop * loop)
	:socket_(socket), state_(kConnecting)
{
	loop_ = loop;
	channel_.reset(new Channel(socket, loop_));

	channel_->SetCloseCallBack(std::bind(&TcpConnection::SolveClose, this));
	channel_->SetReadCallBack(std::bind(&TcpConnection::SolveRead, this));
	channel_->SetWriteCallBack(std::bind(&TcpConnection::SolveWrite, this));
	channel_->SetErrorCallBack(std::bind(&TcpConnection::SolveError, this));

	int optval = 1;
	::setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE,
		&optval, static_cast<socklen_t>(sizeof optval));
}
TcpConnection::~TcpConnection()
{
	assert(state_ == kDisconnected);
	close(socket_);
}
void TcpConnection::send(char* data, int len)
{
	if (state_ == kConnected)
	{
		if (loop_->isInLoopThread())
		{
			SolveSend(data, len);
		}
		else
		{
			loop_->runInLoop(std::bind(&TcpConnection::SolveSend,this,data,len));
		}
	}
}
void TcpConnection::SolveRead()
{
	//直接回声
	//char datas[1024] = { 0 };
	//int read_n = read(socket_, datas, 1024);
	//int prefixionLen = GET_INT16(datas + 2) + 4;
	//LogInfo("receive socket:%d,data is:%s,len:%d",socket_, datas+ prefixionLen, read_n);
	//if (read_n < 0)
	//{
	//	LogInfo("read data error is:%s", strerror(errno));
	//	exit(0);
	//}
	//BaseMsg *msgCreateTurret = BaseMsg::DeCode(datas);
	//if (msgCreateTurret != NULL)
	//{
	//	msgCreateTurret->PrintData();
	//}
	////::send(socket_, datas, read_n, 0);
	//send(datas, read_n);
	//LogInfo("send success");
	//return;
	//协议解析
	//LogInfo("ready to solve read");
	//读取数据进行处理

	loop_->assertInLoopThread();
	int read_len = input_buffer_.ReadData(socket_);
	if (read_len == 0)
	{
		SolveClose();
	}
	else if (read_len > 0)
	{
		//交给业务层处理
		LogInfo("recevie data len is:%d", read_len);
		messageCallback_(this,&input_buffer_);
	}
	else
	{
		//出错
		LogInfo("receive data error:%s", strerror(errno));
		SolveError();
	}
}
void TcpConnection::SolveSend(char *datas, int len)
{
//	printf("in solvesend ");
//	global::PrintData(datas, len);
	loop_->assertInLoopThread();
	int remaining = len;
	bool faultError = false;
	if (state_ == kDisconnected)
	{
		LogInfo("disconnected, give up writing");
		return;
	}
//	printf("is_wrting:%d,has_data:%d\n", channel_->enableWrite(), output_buffer_.HasData());
	if (!channel_->enableWrite() && !output_buffer_.HasData())
	{
		int nwrote = write(socket_, datas, len);
		//LogInfo("write data len is:%d", nwrote);
		if (nwrote >= 0)
		{
			remaining = len - nwrote;
			LogInfo("SolveSend write data len is:%d", nwrote);
		}
		else // nwrote < 0
		{
			nwrote = 0;
			LogInfo("write data error:%s", strerror(errno));
			//写没有发生错误
			if (errno != EWOULDBLOCK)
			{
				if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
				{
					faultError = true;
				}
			}
		}
		if (!faultError && remaining > 0)
		{
			LogInfo("cant write all data set writalble event");
			output_buffer_.AppendData(datas + nwrote, len - nwrote);
			channel_->enableWriting();
		}
	}
}
void TcpConnection::SolveError()
{
	int optval;
	socklen_t optlen = static_cast<socklen_t>(sizeof optval);

	int err = getsockopt(socket_, SOL_SOCKET, SO_ERROR, &optval, &optlen);
	LogInfo("SolveError error is:%s", strerror(err));

	//调用handleClose()关闭连接，回收Channel和fd
	//handleClose();
}
void TcpConnection::SolveWrite()
{
	loop_->assertInLoopThread();
	if (channel_->enableWrite())
	{
		ssize_t n = write(socket_, output_buffer_.GetDatas(), output_buffer_.GetValidDataLen());
		if (n > 0)
		{
			output_buffer_.MoveCurrentIndex(n);
			if (!output_buffer_.HasData())
			{
				channel_->disableWriting();
			}
			if (state_ == kDisconnecting)
			{
				shutdownInLoop();
			}
			LogInfo("in writable event write data len is:%d", n);
		}
		else
		{
			LogInfo("TcpConnection::Write");
		}
	}
}
void TcpConnection::SolveClose()
{
	loop_->assertInLoopThread();
	LogInfo("close one socket");
	//assert(state_ == kConnected || state_ == kDisconnecting);
	// we don't close fd, leave it to dtor, so we can find leaks easily.
	setState(kDisconnected);
	channel_->disableAll();

	TcpConnectionPtr guardThis(shared_from_this());
	connectionCallback_(guardThis);
	// must be the last line
	closeCallback_(guardThis);

	//只处理业务上的关闭，真正的socket fd在TcpConnection析构函数中关闭
	//if (socket_)
	//{
	//    sockets::close(socket_->fd());
	//}
}
void TcpConnection::shutdown()
{
	// FIXME: use compare and swap
	if (state_ == kConnected)
	{
		setState(kDisconnecting);
		// FIXME: shared_from_this()?
		loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
	}
}
void TcpConnection::shutdownInLoop()
{
	loop_->assertInLoopThread();
	if (!channel_->enableWrite())
	{
		if (::shutdown(socket_, SHUT_WR) < 0)
		{
			LogInfo ("sockets::shutdownWrite");
		}
	}
}
void TcpConnection::forceClose()
{
	// FIXME: use compare and swap
	if (state_ == kConnected || state_ == kDisconnecting)
	{
		setState(kDisconnecting);
		loop_->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
	}
}
void TcpConnection::forceCloseInLoop()
{
	loop_->assertInLoopThread();
	if (state_ == kConnected || state_ == kDisconnecting)
	{
		// as if we received 0 byte in handleRead();
		SolveClose();
	}
}
//Nagle算法禁用
void TcpConnection::setTcpNoDelay(bool on)
{
	int optval =1;
	setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
}
void TcpConnection::connectEstablished()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnecting);
	setState(kConnected);
	channel_->tie(shared_from_this());

	//假如正在执行这行代码时，对端关闭了连接
	if (!channel_->enableReading())
	{
		LogInfo("enableReading failed.");
		//setState(kDisconnected);
		SolveClose();
		return;
	}
	//connectionCallback_指向void CTcpListener::OnConnection(const std::shared_ptr<TcpConnection>& conn)
	connectionCallback_(shared_from_this());
}
void TcpConnection::connectDestroyed()
{
	loop_->assertInLoopThread();
	if (state_ == kConnected)
	{
		setState(kDisconnected);
		channel_->disableAll();

		connectionCallback_(shared_from_this());
	}
	channel_->remove();
}
