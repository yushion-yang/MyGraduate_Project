/**
		高性能日志系统 线程写入
 **/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string.h>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>

 //struct FILE;  
#define LogInfo(...) Logger::GetInstance().AddToQueue("INFO", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//Logger::GetInstance().AddToQueue("INFO", __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#define LogWarning(...) Logger::GetInstance().AddToQueue("WARNING", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//Logger::GetInstance().AddToQueue("WARNING", __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#define LogError(...) Logger::GetInstance().AddToQueue("ERROR", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//Logger::GetInstance().AddToQueue("ERROR", __FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)

class Logger
{
public:
	static Logger& GetInstance();

	void SetFileName(const char* filename);
	bool Start();
	void Stop();

	void AddToQueue(const char* pszLevel,
		const char* pszFile,
		int lineNo,
		const char* pszFuncSig,
		const char* pszFmt, ...);

private:
	Logger() = default;
	Logger(const Logger& rhs) = delete;
	Logger& operator =(Logger& rhs) = delete;

	void threadfunc();


private:
	std::string                     filename_;
	FILE*                           fp_{};
	std::shared_ptr<std::thread>    spthread_;
	std::mutex                      mutex_;
	std::condition_variable         cv_;
	//有新的日志到来的标识
	bool                            exit_{ false };
	std::list<std::string>          queue_;
};

#endif //!__LOGGER_H__
