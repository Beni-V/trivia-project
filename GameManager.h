#pragma once
#include "IDatabase.h"
#include "SqliteDatabase.h"
#include "Game.h"
#include "Room.h"

class GameManager
{
private:
	IDAtabase* m_database;
	std::vector<Game> m_games;

public:
	GameManager();
	Game craeteGame(Room room);
	void deleteGame(Game gameToDelete);
};