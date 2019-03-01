#include "Logger.h"
#include <time.h>
#include <stdio.h>
#include <memory>
#include <stdarg.h>
#pragma warning (disable:4996)
//#define YDEBUG	//YODO 如果没有定义宏则采用日志的全文方式显示到屏幕  否则只显示主要内容

Logger& Logger::GetInstance()
{
	static Logger logger;
	return logger;
}

void Logger::SetFileName(const char* filename)
{
	filename_ = filename;
}

bool Logger::Start()
{
	if (filename_.empty())
	{
		time_t now = time(NULL);
		struct tm* t = localtime(&now);
		char timestr[64] = { 0 };
		sprintf(timestr,
			"%04d%02d%02d%02d%02d%02d.imserver.log",
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);
		filename_ = timestr;
	}

	fp_ = fopen(filename_.c_str(), "wt+");
	if (fp_ == NULL)
		return false;

	spthread_.reset(new std::thread(std::bind(&Logger::threadfunc, this)));

	return true;
}

void Logger::Stop()
{
	exit_ = true;
	cv_.notify_one();

	//等待时间线程结束  
	spthread_->join();
}

void Logger::AddToQueue(const char* pszLevel,
	const char* pszFile,
	int lineNo,
	const char* pszFuncSig,
	const char* pszFmt, ...)
{
	char msg[256] = { 0 };

	va_list vArgList;
	va_start(vArgList, pszFmt);
	vsnprintf(msg, 256, pszFmt, vArgList);
	va_end(vArgList);
#ifndef YDEBUG
	printf("%s\n", msg);		//YODO 测试 正式版注释掉
#endif // YDEBUG
	time_t now = time(NULL);
	struct tm* tmstr = localtime(&now);
	char content[512] = { 0 };
	sprintf(content,
		"[%04d-%02d-%02d %02d:%02d:%02d][%s][0x%04x][%s:%d %s]%s\n",
		tmstr->tm_year + 1900,
		tmstr->tm_mon + 1,
		tmstr->tm_mday,
		tmstr->tm_hour,
		tmstr->tm_min,
		tmstr->tm_sec,
		pszLevel,
		std::this_thread::get_id(),
		pszFile,
		lineNo,
		pszFuncSig,
		msg);
	//加上作用域  然后使用锁
	{
		std::lock_guard<std::mutex> guard(mutex_);
		queue_.emplace_back(content);
	}

	cv_.notify_one();
}

void Logger::threadfunc()
{
	if (fp_ == NULL)
		return;

	while (!exit_)
	{
		//写日志  
		std::unique_lock<std::mutex> guard(mutex_);
		while (queue_.empty())
		{
			//在stop中唤醒退出
			if (exit_)
				return;

			cv_.wait(guard);
		}

		//写日志  
		const std::string& str = queue_.front();

		fwrite((void*)str.c_str(), str.length(), 1, fp_);
#ifdef YDEBUG
		printf("%s\n", str.c_str());		
#endif // YDEBUG
		fflush(fp_);
		queue_.pop_front();
	}
}