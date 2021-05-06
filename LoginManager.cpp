#include "LoginManager.h"

LoginManager::LoginManager()
{
	this->m_database = new SqliteDatabase();
}

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
	for (auto user = this->m_loggedUsers.begin(); user != this->m_loggedUsers.end(); ++user)
	{
		if (user->getUsername() == userName)
		{
			this->m_loggedUsers.erase(user);
		}
	}
}
