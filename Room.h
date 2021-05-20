#pragma once

#include <iostream>
#include <vector>
#include "LoggedUser.h"

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	bool isActive;
};

class Room
{
public:
	Room();
	Room(RoomData m_metadata);
	void addUser(LoggedUser newUser);
	void removeUser(LoggedUser user);
	std::vector<std::string> getAllUsers();

private:
	std::vector<LoggedUser> _participates;
	RoomData m_metadata;
};