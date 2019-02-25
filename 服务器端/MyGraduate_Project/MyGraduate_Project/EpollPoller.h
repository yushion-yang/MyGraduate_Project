#ifndef __EPOLL_POLLER_H__
#define __EPOLL_POLLER_H__

#include "Channel.h"
#include "EventLoop.h"

#include <sys/epoll.h>
#include <vector>
#include <map>

class EventLoop;

class EpollPoller
{
	typedef std::vector<struct epoll_event> EventList;
	typedef std::map<int, Channel*> ChannelMap;
	typedef std::vector<Channel*> ChannelList;
public:
	EpollPoller(EventLoop* loop);
	~EpollPoller();

	void poll(int timeoutMs, ChannelList* activeChannels);
	bool updateChannel(Channel* channel);
	void removeChannel(Channel* channel);

	bool hasChannel(Channel* channel) const;

	void assertInLoopThread() const
	{
		ownerLoop_->assertInLoopThread();
	}
private:
	static const int kInitEventListSize = 16;

	void fillActiveChannels(int numEvents,
		ChannelList* activeChannels) const;
	bool update(int operation, Channel* channel);
private:
	int epollfd_;
	EventLoop* ownerLoop_;
	EventList events_;
	ChannelMap channels_;
};
#endif // !__EPOLL_POLLER_H__
