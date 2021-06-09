#include "GameManager.h"

GameManager::GameManager()
{
	this->m_database = new SqliteDatabase();
}

Game GameManager::createGame(Room room)
{
	std::map<LoggedUser, GameData> users;
	std::vector<Question> questions;

	for (std::string user : room.getAllUsers())
	{
		questions.push_back(Question(this->m_database->getQuestions()[0][0], std::vector<std::string>{this->m_database->getQuestions()[0][1], this->m_database->getQuestions()[0][2], this->m_database->getQuestions()[0][3], this->m_database->getQuestions()[0][4]}));
		users.insert({ LoggedUser(user), GameData{Question(this->m_database->getQuestions()[0][0], std::vector<std::string>{this->m_database->getQuestions()[0][1], this->m_database->getQuestions()[0][2], this->m_database->getQuestions()[0][3], this->m_database->getQuestions()[0][4]}), 0, 0, 0} });
	}

	this->m_games.push_back(Game(questions, users));
}
