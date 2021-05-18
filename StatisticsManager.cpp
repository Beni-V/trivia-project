#include "StatisticsManager.h"

StatisticsManager::StatisticsManager()
{
	this->m_database = new SqliteDatabase();
}

std::vector<std::string> StatisticsManager::getHighScore()
{
	std::vector<std::string> users = this->m_database->getUserNames(); // get all user names

	// sort the user names from best score to worst
	for (int i = 0; i < users.size(); i++)
	{
		for (int j = 0; j < users.size() - i - 1; j++)
		{
			if ((float)this->m_database->getNumOfTotalAnswers(users[j]) / (float)this->m_database->getNumOfCorrectAnswers(users[j]) > (float)this->m_database->getNumOfTotalAnswers(users[j + 1]) / (float)this->m_database->getNumOfCorrectAnswers(users[j + 1]))
			{
				std::iter_swap(users.begin() + j, users.begin() + j + 1);
			}
		}
	}

	return std::vector<std::string>(users.begin(), users.begin() + 5); // return top 5 users
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string userName)
{
	// get the stats from DB
	std::string playerAvegageAnswerTime = std::to_string(this->m_database->getPlayerAverageAnswerTime(userName));
	std::string numOfCorrectAnswers = std::to_string(this->m_database->getNumOfCorrectAnswers(userName));
	std::string numOfTotalAnswers = std::to_string(this->m_database->getNumOfTotalAnswers(userName));
	std::string numOfPlayerGames = std::to_string(this->m_database->getNumOfPlayerGames(userName));

	// return the stats in vector
	return std::vector<std::string>({ playerAvegageAnswerTime , numOfCorrectAnswers, numOfTotalAnswers, numOfPlayerGames });
}
