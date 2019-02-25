#include "Lobby.h"

Lobby::Lobby()
{

}

Lobby::~Lobby()
{

}

RoomSession * Lobby::GetRoomSession()
{
	for (int i = 0; i < ROOM_NUM; i++)
	{
		if (!rooms_[i].RsEnoughPlayer())
		{
			return &rooms_[i];
		}
	}
	return nullptr;
}
