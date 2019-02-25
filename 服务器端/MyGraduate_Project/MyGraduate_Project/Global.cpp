#include "Global.h"
#include "Logger.h"
#include <fcntl.h>
#include <cstdlib>
#include <string.h>
//设置非阻塞模式
void global::setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock, F_GETFL);
	if (opts < 0)
	{
		LogError("fcntl(sock,GETFL) error:%s", strerror(errno));
		exit(1);
	}
	opts = opts | O_NONBLOCK;
	if (fcntl(sock, F_SETFL, opts) < 0)
	{
		LogError("fcntl(sock,SETFL,opts) error:%s", strerror(errno));
		exit(1);
	}
}
//打印字符串的前len个字节
void global::PrintData(char * data, int len)
{
	char intent[1024] = { 0 };
	memcpy(intent, data, len);
	printf("data is:%s\n", intent);
}
