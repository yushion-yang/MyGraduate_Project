#ifndef __LOBBY_H__
#define __LOBBY_H__
#include "RoomSession.h"

const int ROOM_NUM = 5000;
class Lobby
{
public:
	Lobby();
	~Lobby();
	RoomSession* GetRoomSession();
private:
	RoomSession rooms_[ROOM_NUM];
};
#endif // !__LOBBY_H__
