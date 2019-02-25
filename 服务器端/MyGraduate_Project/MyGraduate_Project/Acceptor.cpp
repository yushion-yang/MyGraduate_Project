#include "Acceptor.h"
#include "Global.h"
#include "Logger.h"
#include "EpollPoller.h"
#include "EventLoop.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

const int LISTEN_NUM = 10;
const int LISTEN_PORT = 8080;
int socket_num = 0;

Acceptor::Acceptor(EventLoop * loop) : idleFd_(open("/dev/null", O_RDONLY | O_CLOEXEC))
{
	loop_ = loop;
	listen_socket_ = socket(AF_INET, SOCK_STREAM, 0);
	global::setnonblocking(listen_socket_);
	Bind();
//	Listen();
	channel_.reset(new Channel(listen_socket_, loop));
	channel_->SetReadCallBack(std::bind(&Acceptor::Accept, this));
	channel_->enableReading();
}

Acceptor::~Acceptor()
{
	channel_->disableAll();
	channel_->remove();
	close(idleFd_);
}

void Acceptor::Bind()
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(LISTEN_PORT);
	//重用地址
	int opt = 1;
	int re = setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
	if (re < 0)
	{
		LogInfo("SO_REUSEADDR failed.");
	}
	//重用端口
	int optval = 1;
	int ret = ::setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
	if (ret < 0)
	{
		LogInfo("SO_REUSEPORT failed.");
	}
	//绑定
	if (bind(listen_socket_, (sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		LogError("bind failed error:%s", strerror(errno));
		exit(0);
	}
	else
	{
		//LogInfo("bind success");
	}
}

void Acceptor::Listen()
{
	if (listen(listen_socket_, LISTEN_NUM) < 0)
	{
		LogError("listen failed error:%s", strerror(errno));
		exit(0);
	}
	else
	{
		//LogInfo("listen success");
	}
}

void Acceptor::Accept()
{
	LogInfo("Accept new socket loop_:%p",loop_);
	loop_->assertInLoopThread();
	struct sockaddr_in clientaddr;
	socklen_t client_len;
	while (true)
	{
		int new_socket = accept(listen_socket_, (sockaddr*)&clientaddr, &client_len);
		if (new_socket >= 0)
		{
			socket_num++;
			LogInfo("sockets num:%d\n", socket_num);
			global::setnonblocking(new_socket);
			new_connection_call_back_(new_socket);
		}
		else
		{
			if (errno != EAGAIN)
			{
				LogInfo("accept error id is:%d,data is:%s", errno, strerror(errno));
			}
			if (errno == EMFILE)
			{
				close(idleFd_);
				idleFd_ = accept(listen_socket_, NULL, NULL);
				close(idleFd_);
				idleFd_ = open("/dev/null", O_RDONLY | O_CLOEXEC);
			}
			break;
		}
	}
}

void Acceptor::SetNewConnectionCallBack(NewConnectionCallBack new_connection_call_back)
{
	new_connection_call_back_ = new_connection_call_back;
}
