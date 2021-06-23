#include "GameManager.h"

GameManager::GameManager()
{
	this->m_database = new SqliteDatabase();
}

Game GameManager::createGame(Room room)
{
	std::map<std::string, GameData> users;
	std::vector<Question> questions = this->m_database->getQuestions();

	for (std::string user : room.getAllUsers())
	{
		users.insert({ user, GameData{questions[0], 0, 0, 0} });
	}

	this->m_games.push_back(Game(questions, users));

	return Game(questions, users);
}

void GameManager::deleteGame(Game gameToDelete)
{
	this->m_games.pop_back();
}

std::vector<Game> GameManager::getGames()
{
	return this->m_games;
}

void GameManager::submitAnswer(Game game, std::string user,int answerId)
{
	for (int i = 0; i < this->m_games.size(); i++)
	{
		if (this->m_games[i].getUserNames() == game.getUserNames())
		{
			this->m_games[i].submitAnswer(user, answerId);
		}
	}
}
