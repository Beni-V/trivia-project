#include "GameManager.h"

GameManager::GameManager()
{
	this->m_database = new SqliteDatabase();
}

Game GameManager::createGame(Room room)
{
	std::map<std::string, GameData> users;
	std::vector<Question> questions;

	for (std::string user : room.getAllUsers())
	{
		users.insert({ user, GameData{Question(this->m_database->getQuestions()[0][0], std::vector<std::string>{this->m_database->getQuestions()[0][1], this->m_database->getQuestions()[0][2], this->m_database->getQuestions()[0][3], this->m_database->getQuestions()[0][4]}), 0, 0, 0} });
	}
	for (std::vector<std::string> question : this->m_database->getQuestions())
	{
		questions.push_back(Question(question[0], std::vector<std::string>{question[1], question[2], question[3], question[4]}));
	}

	this->m_games.push_back(Game(questions, users));

	return Game(questions, users);
}
