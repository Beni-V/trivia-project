#include "RoomManager.h"

void RoomManager::createRoom(LoggedUser user, RoomData data)
{
	Room newRoom(data);
	newRoom.addUser(user);
	this->m_rooms.insert(std::pair<unsigned int, Room>(data.id, newRoom));
}

void RoomManager::deleteRoom(int ID)
{
	this->m_rooms.erase(ID);
}

bool RoomManager::getRoomState(int ID)
{
	return this->m_rooms.find(ID) != this->m_rooms.end();
}

std::map<unsigned int, Room> RoomManager::getRooms()
{
	return this->m_rooms;
}
