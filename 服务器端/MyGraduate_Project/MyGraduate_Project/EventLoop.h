#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include "TimerQueue.h"
#include "TimerId.h"

#include <thread>
#include <vector>
#include <mutex>

class EpollPoller;
class Channel;

class EventLoop
{
	typedef std::function<void()> Functor;
	typedef std::vector<Channel*> ChannelList;
	typedef std::function<void()> TimerCallback;
public:
	EventLoop();
	~EventLoop();  // force out-line dtor, for scoped_ptr members.
	void loop();
	void quit();
	int64_t iteration() const { return iteration_; }
	void runInLoop(const Functor& cb);
	void queueInLoop(const Functor& cb);

	void setFrameFunctor(const Functor& cb);

	// internal usage
	void wakeup();
	void removeChannel(Channel* channel);
	bool hasChannel(Channel* channel);

	// pid_t threadId() const { return threadId_; }
	void assertInLoopThread()
	{
		if (!isInLoopThread())
		{
			abortNotInLoopThread();
		}
	}
	//判断循环是否在线程中
	bool isInLoopThread() const { return threadId_ == std::this_thread::get_id(); }
	// bool callingPendingFunctors() const { return callingPendingFunctors_; }
	bool eventHandling() const { return eventHandling_; }

	static EventLoop* getEventLoopOfCurrentThread();

	const std::thread::id getThreadID() const
	{
		return threadId_;
	}

	EpollPoller* GetEpollPoller() { return poller_.get(); }
	void SetFrameFunctor(Functor functor) { frameFunctor_ = functor; }
private:
	void abortNotInLoopThread();
	void handleRead();  // waked up
	void doPendingFunctors();

private:

	bool                                looping_; /* atomic */
	bool                                quit_; /* atomic and shared between threads, okay on x86, I guess. */
	bool                                eventHandling_; /* atomic */
	//是否正在调用未决的函数
	bool                                callingPendingFunctors_; /* atomic */
	int64_t                             iteration_;
	const std::thread::id               threadId_;
	std::shared_ptr<EpollPoller>        poller_;
	std::shared_ptr<TimerQueue>         timerQueue_;

	int wakeupFd_;
	// unlike in TimerQueue, which is an internal class,
	// we don't expose Channel to client.
	//唤醒通道
	std::shared_ptr<Channel>            wakeupChannel_;

	// scratch variables
	ChannelList                         activeChannels_;
	Channel*                            currentActiveChannel_;

	std::mutex                          mutex_;
	//等待执行的任务
	std::vector<Functor>                pendingFunctors_; // Guarded by mutex_

	Functor                             frameFunctor_;		//每帧执行的任务（每次epoll_wait返回）
};

#endif // !__EVENT_LOOP_H__
