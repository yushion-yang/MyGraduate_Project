#include"EventLoop.h"
#include "Logger.h"
#include "TimerQueue.h"
#include "Channel.h"
#include "EpollPoller.h"

#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <assert.h>

namespace
{
	__thread EventLoop* t_loopInThisThread = 0;

	const int kPollTimeMs = 1;

	int createEventfd()
	{
		int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
		if (evtfd < 0)
		{
			LogInfo("Failed in eventfd");
			abort();
		}
		return evtfd;
	}

#pragma GCC diagnostic ignored "-Wold-style-cast"
	class IgnoreSigPipe
	{
	public:
		IgnoreSigPipe()
		{
			::signal(SIGPIPE, SIG_IGN);
			// LOG_TRACE << "Ignore SIGPIPE";
		}
	};
#pragma GCC diagnostic error "-Wold-style-cast"

	IgnoreSigPipe initObj;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
	return t_loopInThisThread;
}

// 在线程函数中创建eventloop
EventLoop::EventLoop()
	: looping_(false),
	quit_(false),
	eventHandling_(false),
	callingPendingFunctors_(false),
	iteration_(0),
	threadId_(std::this_thread::get_id()),
	poller_(new EpollPoller(this)),
	timerQueue_(new TimerQueue(this)),
	wakeupFd_(createEventfd()),
	wakeupChannel_(new Channel(wakeupFd_, this)),
	currentActiveChannel_(NULL)
{
	if (t_loopInThisThread)
	{
		LogInfo("Another EventLoop  exists in this thread ");
	}
	else
	{
		t_loopInThisThread = this;
	}
	wakeupChannel_->SetReadCallBack(std::bind(&EventLoop::handleRead, this));
	// we are always reading the wakeupfd
	wakeupChannel_->enableReading();

	//std::stringstream ss;	
	//ss << "eventloop create threadid = " << threadId_;
	//std::cout << ss.str() << std::endl;
}

EventLoop::~EventLoop()
{
	assertInLoopThread();
	//LOG_DEBUG << "EventLoop destructs in other thread";

	//std::stringstream ss;
	//ss << "eventloop destructs threadid = " << threadId_;
	//std::cout << ss.str() << std::endl;

	wakeupChannel_->disableAll();
	wakeupChannel_->remove();
	::close(wakeupFd_);
	t_loopInThisThread = NULL;
}
//yys while主循环
void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	quit_ = false;  // FIXME: what if someone calls quit() before loop() ?
	LogInfo("EventLoop start looping");

	while (!quit_)
	{
		activeChannels_.clear();
		//分离事件 poller_->poll利用epoll分离网络事件，然后接着处理分离出来的网络事件，每一个客户端\
		socket对应一个连接，即一个TcpConnection和Channel通道对象。
		poller_->poll(kPollTimeMs, &activeChannels_);
		++iteration_;
		eventHandling_ = true;
		for (ChannelList::iterator it = activeChannels_.begin();
			it != activeChannels_.end(); ++it)
		{
			currentActiveChannel_ = *it;
			//currentActiveChannel_->handleEvent(pollReturnTime_)根据是可读、可写、出错事件来调用对应的\
			处理函数，这些函数都是回调函数，程序初始化阶段设置进来的：
			currentActiveChannel_->SolveEvent();
		}
		currentActiveChannel_ = NULL;
		eventHandling_ = false;
		doPendingFunctors();
		//yys 而frameFunctor_就更简单了，就是通过设置一个函数指针就可以了。当然这里有个技巧性的东西，\
		即增加任务的时候，为了能够立即执行，使用唤醒机制，通过往一个fd里面写入简单的几个字节，来唤醒epoll，\
		使其立刻返回，因为此时没有其它的socke有事件，这样接下来就执行刚才添加的任务了。
		if (frameFunctor_)
		{
			frameFunctor_();
		}
	}

	LogInfo("EventLoop stop looping");
	looping_ = false;
}
//退出线程
void EventLoop::quit()
{
	quit_ = true;
	// There is a chance that loop() just executes while(!quit_) and exists,
	// then EventLoop destructs, then we are accessing an invalid object.
	// Can be fixed using mutex_ in both places.
	if (!isInLoopThread())
	{
		wakeup();
	}
}
//添加任务
void EventLoop::runInLoop(const Functor& cb)
{
	//如果是当前线程添加的则直接执行
	if (isInLoopThread())
	{
		cb();
	}
	//否则添加到任务对列中
	else
	{
		queueInLoop(cb);
	}
}
//yys 增加任务的地方	可以由其他线程调用
void EventLoop::queueInLoop(const Functor& cb)
{
	//添加任务	可能多个线程调用
	{
		std::unique_lock<std::mutex> lock(mutex_);
		pendingFunctors_.push_back(cb);
	}

	//不是当前线程或者或者正在执行额外的任务时唤醒
	if (!isInLoopThread() || callingPendingFunctors_)
	{
		wakeup();
	}
}
//设置帧回调函数
void EventLoop::setFrameFunctor(const Functor& cb)
{
	frameFunctor_ = cb;
}
//移除一个channel
void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	if (eventHandling_)
	{
	/*	assert(currentActiveChannel_ == channel ||
			std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());*/
	}

	LogInfo("Remove channel, channel fd = %d", channel->fd());
	poller_->removeChannel(channel);
}
//判断是否存在某个channel
bool EventLoop::hasChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	return poller_->hasChannel(channel);
}
//警告非当前线程执行	对于某些循环对象的函数 只能在包含它的线程中执行	比如清除销毁对象
void EventLoop::abortNotInLoopThread()
{
	std::stringstream ss;
	ss << "threadid_ = " << threadId_ << " this_thread::get_id() = " << std::this_thread::get_id();
	LogInfo("EventLoop::abortNotInLoopThread - EventLoop %s",ss.str().c_str());
}
//写入一些内容将线程唤醒
void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = write(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		LogInfo("EventLoop::wakeup() writes %d bytes instead of 8",n);
	}
}
//读取唤醒事件写入的字符
void EventLoop::handleRead()
{
	LogInfo("Loop wakuped by other thread");
	uint64_t one = 1;
	ssize_t n = read(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		LogInfo("EventLoop::handleRead() reads %d bytes instead of 8",n);
	}
}
//yys 主循环里面的业务逻辑处理对应：
void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	callingPendingFunctors_ = true;

	{
		std::unique_lock<std::mutex> lock(mutex_);
		functors.swap(pendingFunctors_);
		/*
		这里增加业务逻辑是增加执行任务的函数指针的，增加的任务保存在成员变量pendingFunctors_中，这个变量
		是一个函数指针数组（vector对象），执行的时候，调用每个函数就可以了。上面的代码先利用一个栈变量将
		成员变量pendingFunctors_里面的函数指针换过来，接下来对这个栈变量进行操作就可以了，这样减少了锁的
		粒度。因为成员变量pendingFunctors_在增加任务的时候，也会被用到，设计到多个线程操作，所以要加锁
		*/
	}//退出此作用域  其他线程此时就可以往此线程添加任务了

	for (size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}
	callingPendingFunctors_ = false;
}