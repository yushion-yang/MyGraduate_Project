#ifndef __PLAYER_H__
#define __PLAYER_H__
class TcpConnection;
class Buffer;
class Player
{
public:
	Player();
	~Player();
	void SendData(char *datas,int len);
	void SolveData(Buffer* buffer);
	void SolveData(char* datas,int len);
	void SetTcpConnection(TcpConnection* conn);
private:
	TcpConnection* conn_;
};

#endif // !__PLAYER_H__
