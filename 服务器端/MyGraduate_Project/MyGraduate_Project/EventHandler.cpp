#include "EventHandler.h"
#include "Logger.h"

EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}

void EventHandler::OnDisconnect(ClientState * state)
{
	LogInfo("OnDisconnect");
	//Console.WriteLine("Close");
	////Player下线
	//if (c.player != null) {
	//	//离开战场
	//	int roomId = c.player.roomId;
	//	if (roomId >= 0) {
	//		Room room = RoomManager.GetRoom(roomId);
	//		room.RemovePlayer(c.player.id);
	//	}
	//	//保存数据
	//	DbManager.UpdatePlayerData(c.player.id, c.player.data);
	//	//移除
	//	PlayerManager.RemovePlayer(c.player.id);
	//}
}
