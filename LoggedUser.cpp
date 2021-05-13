#include "LoggedUser.h"

LoggedUser::LoggedUser(std::string username)
{
	this->m_username = username;
}

std::string LoggedUser::getUsername()
{
	return this->m_username;
}

bool LoggedUser::operator==(const LoggedUser& user) const
{
	return this->m_username == user.m_username;
}

