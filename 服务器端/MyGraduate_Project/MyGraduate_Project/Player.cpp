#include "Player.h"
#include "TcpConnection.h"
#include "Logger.h"
#include <string.h>

Player::Player()
{

}

Player::~Player()
{

}

void Player::SendData(char *datas,int len)
{
	conn_->SolveSend(datas+4,len-4);
}

void Player::SolveData(Buffer * buffer)
{
	while (true)
	{
		if (buffer->EnoughHeadLen())
		{
			LogInfo("EnoughHeadLen");
			buffer->SetSolveDataLen();
		}
		else
		{
			break;
		}
		if (buffer->EnoughOnePackageLen())
		{
			LogInfo("EnoughOnePackageLen");
			int solve_data_len = buffer->GetPackageLen();
			char intent[100] = { 0 };
			memcpy(intent, buffer->GetDatas() + 4, solve_data_len - 4);
			printf("solve data is:%s\n", intent);
			SendData(buffer->GetDatas(), solve_data_len);
			buffer->SolveLen();
		}
		else
		{
			break;
		}
	}
}

void Player::SolveData(char * datas,int len)
{
	SendData(datas, len);
}

void Player::SetTcpConnection(TcpConnection * conn)
{
	conn_ = conn;
}
