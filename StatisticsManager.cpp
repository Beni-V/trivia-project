#include "StatisticsManager.h"

StatisticsManager::StatisticsManager()
{
	this->m_database = new SqliteDatabase();
}

std::vector<std::string> StatisticsManager::getHighScore()
{
	std::vector<std::string> userNames = this->m_database->getUserNames();

	for (int i = 0; i < userNames.size(); i++)
	{
		for (int j = 0; j < userNames.size() - i - 1; j++)
		{
			if ((float)this->m_database->getNumOfTotalAnswers(userNames[j]) / (float)this->m_database->getNumOfCorrectAnswers(userNames[j]) > (float)this->m_database->getNumOfTotalAnswers(userNames[j + 1]) / (float)this->m_database->getNumOfCorrectAnswers(userNames[j + 1]))
			{
				std::iter_swap(userNames.begin() + j, userNames.begin() + j + 1);
			}
		}
	}
	if (userNames.size() > 3)
	{
		return std::vector<std::string>(userNames.begin(), userNames.begin() + 3);
	}
	else
	{
		return userNames;
	}
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string userName)
{
	// gte the stats from DB
	std::string playerAvegageAnswerTime = std::to_string(this->m_database->getPlayerAverageAnswerTime(userName));
	std::string numOfCorrectAnswers = std::to_string(this->m_database->getNumOfCorrectAnswers(userName));
	std::string numOfTotalAnswers = std::to_string(this->m_database->getNumOfTotalAnswers(userName));
	std::string numOfPlayerGames = std::to_string(this->m_database->getNumOfPlayerGames(userName));

	// return the stats in vector
	return std::vector<std::string>({ playerAvegageAnswerTime , numOfCorrectAnswers, numOfTotalAnswers, numOfPlayerGames });
}
