#pragma once

#include <iostream>
#include "Room.h"
#include <map>

class RoomManager
{
public:
	void createRoom(LoggedUser user, RoomData data);
	void deleteRoom(int ID);
	bool getRoomState(int ID);
	std::map<unsigned int, Room> getRooms();
	
private:
	std::map<unsigned int, Room> m_rooms;
};