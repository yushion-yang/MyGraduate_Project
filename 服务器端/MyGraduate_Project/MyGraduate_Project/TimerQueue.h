#ifndef __TIMER_QUEUE_H__
#define __TIMER_QUEUE_H__

class EventLoop;

class TimerQueue
{
public:
	TimerQueue(EventLoop* loop);
	~TimerQueue();

private:
	EventLoop* loop_;
};

#endif // !__TIMER_QUEUE_H__
