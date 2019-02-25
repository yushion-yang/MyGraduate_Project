#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Channel.h"

#include <memory>

class EventLoop;
class EpollPoller;

class Acceptor
{
	using NewConnectionCallBack = std::function<void(int socket)>;
public:
	Acceptor(EventLoop* loop);
	~Acceptor();
	void Bind();
	void Listen();
	void Accept();
	void SetNewConnectionCallBack(NewConnectionCallBack new_connection_call_back);
private:
	int listen_socket_;
	EventLoop* loop_;
	EpollPoller* epoller;
	std::shared_ptr<Channel> channel_;
	NewConnectionCallBack new_connection_call_back_;
	int idleFd_;
};

#endif // !__ACCEPTOR_H__
