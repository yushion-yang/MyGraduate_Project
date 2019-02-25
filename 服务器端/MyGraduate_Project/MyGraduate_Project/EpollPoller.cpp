#include "EpollPoller.h"
#include "Logger.h"
#include "Global.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sstream>

namespace
{
	const int kNew = -1;		//新的channel
	const int kAdded = 1;		//已经添加的channel
	const int kDeleted = 2;		//准备被删除的channel
}

EpollPoller::EpollPoller(EventLoop* loop)
	:events_(kInitEventListSize),
	ownerLoop_(loop)
{
	epollfd_ = epoll_create(256);
	if (epollfd_ < 0)
	{
		LogInfo("EpollPoller::EpollPoller");
	}
}

EpollPoller::~EpollPoller()
{
	::close(epollfd_);
}
//判断是否存在channel
bool EpollPoller::hasChannel(Channel* channel) const
{
	assertInLoopThread();
	ChannelMap::const_iterator it = channels_.find(channel->fd());
	return it != channels_.end() && it->second == channel;
}

void EpollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
	int numEvents = ::epoll_wait(epollfd_,&*events_.begin(),static_cast<int>(events_.size()),timeoutMs);

	int savedErrno = errno;
	if (numEvents > 0)
	{
		std::stringstream ss;
		ss << "this_thread::get_id() = " << std::this_thread::get_id();
		LogInfo("%d events happended %s", numEvents, ss.str().c_str());
		fillActiveChannels(numEvents, activeChannels);
		//增大预设的事件数量
		if (static_cast<size_t>(numEvents) == events_.size())
		{
			events_.resize(events_.size() * 2);
		}
	}
	else if (numEvents == 0)
	{
		//LOG_TRACE << " nothing happended";
	}
	else
	{
		// error happens, log uncommon ones
		if (savedErrno != EINTR)
		{
			errno = savedErrno;
			LogInfo("EpollPoller::poll()");
		}
	}
}
//填充channel
void EpollPoller::fillActiveChannels(int numEvents,ChannelList* activeChannels) const
{
	assert( numEvents <= events_.size());
	for (int i = 0; i < numEvents; ++i)
	{
		Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
		//设置接收事件
		channel->set_revents(events_[i].events);	
		activeChannels->push_back(channel);
	}
}
//对channel进行event处理
bool EpollPoller::updateChannel(Channel* channel)
{
	assertInLoopThread();
	LogInfo("updateChannel fd = %d events = %d",channel->fd(),channel->events());
	const int index = channel->index();
	//没有存在事件 进行添加操作
	int fd = channel->fd();
	if (index == kNew || index == kDeleted)
	{
		// a new one, add with EPOLL_CTL_ADD
		if (index == kNew)
		{
			//assert(channels_.find(fd) == channels_.end())
			if (channels_.find(fd) != channels_.end())
			{
				LogInfo( "fd = %d must not exist in channels_",fd);
				return false;
			}
			//将channel加入到map中
			channels_[fd] = channel;
		}
		else // index == kDeleted
		{
			//不存在
			if (channels_.find(fd) == channels_.end())
			{
				LogInfo("fd = %d must exist in channels_", fd);
				return false;
			}

			//不匹配
			if (channels_[fd] != channel)
			{
				LogInfo("current channel is not matched current fd, fd =", fd);
				return false;
			}
		}
		channel->set_index(kAdded);

		return update(EPOLL_CTL_ADD, channel);
	}
	//存在事件 进行删除/修改操作
	else
	{
		// update existing one with EPOLL_CTL_MOD/DEL
		//assert(channels_.find(fd) != channels_.end());
		//assert(channels_[fd] == channel);
		//assert(index == kAdded);
		//不存在 不匹配channel 不匹配事件
		if (channels_.find(fd) == channels_.end() || channels_[fd] != channel || index != kAdded)
		{
			LogInfo("current channel is not matched current fd, fd = %d channel = ", fd);
			return false;
		}
		//判断是否不存在事件
		if (channel->isNoneEvent())
		{
			//不存在则移除事件 并标记channel为被删除的
			if (update(EPOLL_CTL_DEL, channel))
			{
				channel->set_index(kDeleted);
				return true;
			}
			return false;
		}
		//存在则修改
		else
		{
			return update(EPOLL_CTL_MOD, channel);
		}
	}
}
//移除channel
void EpollPoller::removeChannel(Channel* channel)
{
	assertInLoopThread();
	int fd = channel->fd();
	LogInfo("remove channel fd =%d",fd);
	assert(channels_.find(fd) != channels_.end());		//不存在
	assert(channels_[fd] == channel);					//不匹配
	assert(channel->isNoneEvent());						//存在事件
	int index = channel->index();
	assert(index == kAdded || index == kDeleted);		//不是添加的或者删除的
	size_t n = channels_.erase(fd);						//在map中删除channel
	(void)n;
	assert(n == 1);

	if (index == kAdded)								//清除事件
	{
		update(EPOLL_CTL_DEL, channel);
	}
	//重设为新的channel
	channel->set_index(kNew);
}

//yys 设置EPOLL事件
bool EpollPoller::update(int operation, Channel* channel)
{
	struct epoll_event event;
	bzero(&event, sizeof event);
	event.events = channel->events();
	event.data.ptr = channel;
	int fd = channel->fd();
	if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
	{
		LogInfo("epoll_ctl op=%d fd=%d, epollfd=%d, errno=%d, errorInfo:%s", operation, fd, epollfd_, errno, strerror(errno));
		return false;
	}
	return true;
}
