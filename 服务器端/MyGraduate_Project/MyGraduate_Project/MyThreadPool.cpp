#include "MyThreadPool.h"
#include "EventLoop.h"
#include <assert.h>

MyThreadPool::MyThreadPool()
{

}

MyThreadPool::~MyThreadPool()
{
	//等待所有线程退出
	if (!is_stop_)
	{
		Stop();
	}
	//删除创建的loop
	for (auto& loop : loops_)
	{
		delete loop;
	}
	//清空所有任务
	tasks_.clear();
}

void MyThreadPool::Init(EventLoop* base_loop,int max_thread_count)
{
	//线程已结束
	assert(!is_stop_);

	current_use_thread_index_ = 0;
	is_stop_ = false;
	max_thread_count_ = max_thread_count < 1 ? 0 : max_thread_count;
	relaxed_thread_count_ = max_thread_count_;
	base_loop_ = base_loop;
}

void MyThreadPool::Start()
{
	//线程已结束
	assert(!is_stop_);
	//线程未初始化
	assert(base_loop_);
	for (int i = 0; i < max_thread_count_; i++)
	{
		threads_.emplace_back(std::bind(&MyThreadPool::Run, this));
	}
}

bool MyThreadPool::Run()
{
	while (!is_stop_)
	{
		EventLoop* loop = new EventLoop();
		{
			std::unique_lock<std::mutex> lk(mutx_);
			loops_.push_back(loop);
		}
		loop->loop();
		////等待 判断退出 执行清除
		//FunCallBack fcb;
		//{
		//	std::unique_lock<std::mutex> lk(mutx_);

		//	cond_.wait(lk, [&]()->bool {return !tasks_.empty() || is_stop_; });

		//	if (is_stop_)
		//	{
		//		break;
		//	}
		//	//fcb = std::move(tasks_.back());
		//	fcb = tasks_.back();
		//	tasks_.pop_back();
		//}
		//relaxed_thread_count_--;
		//fcb();
		//relaxed_thread_count_++;
	}
	return false;	//unable do this
}

//可以反复调用
bool MyThreadPool::Stop()
{
	is_stop_ = true;
	cond_.notify_all();
	for (auto& t : threads_)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	return true;
}

EventLoop * MyThreadPool::GetNextLoop()
{
	//线程已终止
	assert(!is_stop_);
	//线程未开启
	assert(base_loop_);
	//主线程留做监听用途 不参与IO处理
	if (max_thread_count_ == 0)
	{
		return base_loop_;
	}
	current_use_thread_index_++;
	if (current_use_thread_index_ == max_thread_count_)
	{
		current_use_thread_index_ = 0;
	}
	return loops_[current_use_thread_index_];
}


