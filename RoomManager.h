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
	void addUserToRoom(int id, LoggedUser user);
	void removeUserFromRoom(int id, LoggedUser user);
	std::map<unsigned int, Room> getRooms();
	
private:
	std::map<unsigned int, Room> m_rooms;
};