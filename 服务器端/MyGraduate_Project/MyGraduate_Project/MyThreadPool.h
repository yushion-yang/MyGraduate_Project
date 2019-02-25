#ifndef __MY_THREAD_POOL__
#define __MY_THREAD_POOL__

#include <vector>
#include <functional>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <iostream>
#include <condition_variable>

class EventLoop;

class MyThreadPool
{
	using FunCallBack = std::function<void()>;
public:
	MyThreadPool();
	~MyThreadPool();
	void Init(EventLoop* base_loop,int max_thread_count);
	void Start();
	bool Run();
	bool Stop();
	int GetRelaxedThreadCnt() { return relaxed_thread_count_; }

	template<class F, class...Args>
	void AddTask(F&& f, Args...args)
	{
		if (is_stop_)
		{
			throw std::runtime_error("add task on ThreadPool is stopped.");
		}
		std::lock_guard<std::mutex> lk(mutx_);
		tasks_.emplace_back(std::bind(f, std::forward<Args>(args)...));
		cond_.notify_one();
	}

	EventLoop* GetNextLoop();
private:
	bool is_stop_ = false;
	int max_thread_count_ = 0;
	std::atomic<int> relaxed_thread_count_ ;
	std::vector<std::thread> threads_ = {};
	std::vector<FunCallBack> tasks_ = {};
	std::mutex mutx_;
	std::condition_variable cond_;
	std::vector<EventLoop*> loops_ = {};
	EventLoop* base_loop_;
	std::atomic<int> current_use_thread_index_ ;
};
#endif // !__MY_THREAD_POOL__

