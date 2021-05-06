#include "LoginManager.h"

void LoginManager::signup(std::string userName, std::string password, std::string email)
{
	this->m_database->addNewUser(userName, password, email);
}

void LoginManager::login(std::string userName, std::string password)
{
	LoggedUser newUser(userName);
	if (this->m_database->doesPasswordMatch(userName, password))
	{
		this->m_loggedUsers.push_back(newUser);
	}
}

void LoginManager::logout(std::string userName)
{
	this->m_loggedUsers.erase(std::remove(m_loggedUsers.begin(), m_loggedUsers.end(), userName));
}
