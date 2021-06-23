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
	Game createGame(Room room);
	void deleteGame(Game gameToDelete);
	std::vector<Game> getGames();
	void submitAnswer(Game game, std::string user, int answerId);
};