#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "Channel.h"
#include "Buffer.h"
#include "Timestamp.h"
#include "Callbacks.h"

#include <memory>
#include <atomic>
#include <string>

//断开连接 连接中 已连接 断开连接中
enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };

class TcpConnection:public std::enable_shared_from_this<TcpConnection>
{
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void(TcpConnection*, Buffer*)> MessageCallback;
public:
	TcpConnection(int socket,EventLoop* loop);
	~TcpConnection();

	void SolveRead();
	void SolveSend(char *datas,int len);
	void SolveClose();
	void SolveError();
	void SolveWrite();

	void send(char* message, int len);
	void shutdown();
	void forceClose();
	void setTcpNoDelay(bool on);

	void setMessageCallback(const MessageCallback& cb){	messageCallback_ = cb;}
	void setConnectionCallback(const ConnectionCallback& cb){connectionCallback_ = cb;}
	void setWriteCompleteCallback(const WriteCompleteCallback& cb){	writeCompleteCallback_ = cb;}
	void setCloseCallback(const CloseCallback& cb){ closeCallback_ = cb;}
	Buffer* inputBuffer(){ return &input_buffer_;}
	Buffer* outputBuffer(){	return &output_buffer_;	}
	EventLoop* getLoop() const { return loop_; }
	bool connected() const { return state_ == kConnected; }

	void connectEstablished();
	void connectDestroyed();
private:
	void shutdownInLoop();
	void forceCloseInLoop();
	void setState(StateE s) { state_ = s; }
public:
	int socket_;
	std::shared_ptr<Channel> channel_;
	EventLoop* loop_;
	Buffer input_buffer_;
	Buffer output_buffer_;
	std::atomic<StateE> state_;
	MessageCallback messageCallback_;
	ConnectionCallback connectionCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	CloseCallback closeCallback_;
	HighWaterMarkCallback highWaterMarkCallback_;
};
#endif // !__TCP_CONNECTION_H__
