#include "EventLoop.h"
#include "IMServer.h"
#include "Singleton.h"
#include "Logger.h"
#include "MyThreadPool.h"
#include "DaemonRun.h"
#include "NetManager.h"

#include <signal.h>
#include <sys/stat.h>
#include <getopt.h>

void prog_exit(int signo)
{
	Singleton<MyThreadPool>::Instance().Stop();
}

const int THREAD_NUM = 4;

int main(int argc, char* argv[])
{
	signal(SIGCHLD, SIG_DFL);		//默认处理方式
	signal(SIGPIPE, SIG_IGN);		//忽略信号
	signal(SIGINT, prog_exit);
	signal(SIGTERM, prog_exit);

	int ch;
	bool bdaemon = false;
	//设置了-d命令
	while ((ch = getopt(argc, argv, "d")) != -1)
	{
		switch (ch)
		{
		case 'd':
			bdaemon = true;
			break;
		}
	}
	//守护进行方式启动
	if (bdaemon)
	{
		daemon_run();
	}

	Logger::GetInstance().SetFileName("log.txt");
	Logger::GetInstance().Start();
	//TODO 主循环 设置每帧执行的任务为检测连接心跳是否到期 
	//任务由业务逻辑层来完成 可以使用静态类NetManager  此类包括处理心跳时间方法
	EventLoop main_loop;		

	Singleton<NetManager>::Instance();

	Singleton<MyThreadPool>::Instance().Init(&main_loop,THREAD_NUM);
	Singleton<MyThreadPool>::Instance().Start();

	Singleton<IMServer>::Instance().Init(&main_loop);

	LogInfo("chatserver initialization completed, now you can use client to connect it.");
	
	main_loop.loop();

	LogInfo("exit chatserver.");

	return 0;
}
