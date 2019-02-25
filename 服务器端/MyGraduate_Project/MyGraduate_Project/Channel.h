#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "Timestamp.h"

#include <functional>
#include <memory>

class EventLoop;
class EpollPoller;

class Channel
{
	using FunCallBack = std::function<void()>;
	using WriteCallBack = std::function<void(char*, int)>;
public:
	Channel(int socket,EventLoop* loop);
	~Channel();
	void SetReadCallBack(const FunCallBack& read_call_back);
	void SetWriteCallBack(const FunCallBack& write_call_back);
	void SetCloseCallBack(const FunCallBack& close_call_back);
	void SetErrorCallBack(const FunCallBack& error_call_back);

	void SolveEvent();
	int GetEvent() { return events_; }

	int GetSocket() { return socket_; }
	bool enableWrite() const { return events_ & kWriteEvent; }
	void tie(const std::shared_ptr<void>& obj);

	//绑定
	int fd() const { return socket_; }
	int events() const { return events_; }
	//设置事件类型
	void set_revents(int revt) { revents_ = revt; } // used by pollers
	// int revents() const { return revents_; }
	bool isNoneEvent() const { return events_ == kNoneEvent; }

	bool enableReading();
	bool disableReading();
	bool enableWriting();
	bool disableWriting();
	bool disableAll();
	void remove();

	// for Poller
	int index() { return index_; }
	void set_index(int idx) { index_ = idx; }

	void doNotLogHup() { logHup_ = false; }

	EventLoop* ownerLoop() { return loop_; }
private:
	void handleEventWithGuard();

	static const int            kNoneEvent;
	static const int            kReadEvent;
	static const int            kWriteEvent;

	//事件类型
	int                         revents_; // it's the received event types of epoll or poll
	int                         index_; // used by Poller.
	bool                        logHup_;

	std::weak_ptr<void>         tie_;  //std::shared_ptr<void>/std::shared_ptr<void>可以指向不同的数据类型
	bool                        tied_;

	int events_ = 0;
	int socket_;
	EventLoop* loop_;
	EpollPoller* poller_;
	FunCallBack read_call_back_;
	FunCallBack write_call_back_;
	FunCallBack close_call_back_;
	FunCallBack error_call_back_;
};
#endif // !__CHANNEL_H__
