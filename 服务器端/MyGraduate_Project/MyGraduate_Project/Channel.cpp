#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include "Global.h"
#include "EpollPoller.h"

#include <poll.h>
#include <assert.h>
#include <sys/epoll.h>
#include <sstream>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT ;

Channel::Channel(int socket, EventLoop * loop)
	: loop_(loop),
	socket_(socket),
	events_(0),
	revents_(0),
	index_(-1),
	logHup_(true),
	tied_(false)
{
	//默认使用ET模式 存放EpollPoller来直接更新channel事件
	poller_ = loop_->GetEpollPoller();
	events_ |= EPOLLET;
}

Channel::~Channel()
{

}

void Channel::tie(const std::shared_ptr<void>& obj)
{
	tie_ = obj;
	tied_ = true;
}

bool Channel::enableReading()
{
	events_ |= kReadEvent;
	return poller_->updateChannel(this);
}

bool Channel::disableReading()
{
	events_ &= ~kReadEvent;
	return poller_->updateChannel(this);
}

bool Channel::enableWriting()
{
	events_ |= kWriteEvent;
	return poller_->updateChannel(this);
}

bool Channel::disableWriting()
{
	events_ &= ~kWriteEvent;
	return poller_->updateChannel(this);
}

bool Channel::disableAll()
{
	events_ = kNoneEvent;
	return poller_->updateChannel(this);
}

void Channel::remove()
{
	assert(isNoneEvent());
	loop_->removeChannel(this);
}

void Channel::SolveEvent()
{
	std::shared_ptr<void> guard;
	if (tied_)
	{
		guard = tie_.lock();
		if (guard)
		{
			handleEventWithGuard();
		}
	}
	else
	{
		handleEventWithGuard();
	}
}

void Channel::handleEventWithGuard()
{
	if ((events_ & EPOLLHUP) && !(events_ & EPOLLIN))
	{
		if (close_call_back_)
		{
			LogInfo("Channel::handle_event() EPOLLHUP ");
			close_call_back_();
		}
	}
	if (events_ & POLLNVAL)
	{
		LogInfo("Channel::handle_event() POLLNVAL");
	}
	if (events_ & (EPOLLERR | POLLNVAL))
	{
		//接收出错时回调
		if (error_call_back_)
		{
			LogInfo("Channel::handle_event() POLLERR POLLNVAL");
			error_call_back_();
		}
	}
	if (events_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		//当是侦听socket时，readCallback_指向Acceptor::handleRead
		//当是客户端socket时，调用TcpConnection::handleRead 
		if (read_call_back_)
		{
			//可读的事件回调
			LogInfo("solve event read");
			read_call_back_();
		}
	}
	if (events_ & EPOLLOUT)
	{
		//如果是连接状态服的socket，则writeCallback_指向Connector::handleWrite()
		if (write_call_back_)
		{
			//可写的事件回调
			LogInfo("solve event write");
			write_call_back_();
		}
	}
}

void Channel::SetReadCallBack(const FunCallBack& read_call_back)
{
	read_call_back_ = read_call_back;
}

void Channel::SetWriteCallBack(const FunCallBack& write_call_back)
{
	write_call_back_ = write_call_back;
}

void Channel::SetCloseCallBack(const FunCallBack& close_call_back)
{
	close_call_back_ = close_call_back;
}

void Channel::SetErrorCallBack(const FunCallBack & error_call_back)
{
	error_call_back_ = error_call_back;
}
