#include "Room.h"

Room::Room()
{
}

Room::Room(RoomData m_metadata)
    : m_metadata(m_metadata)
{}

void Room::addUser(LoggedUser newUser)
{
    if (std::find(this->_participates.begin(), this->_participates.end(), newUser) == this->_participates.end())
    {
        this->_participates.push_back(newUser);
    }
}

void Room::removeUser(LoggedUser user)
{
    std::vector<LoggedUser>::iterator it;
    for (it = this->_participates.begin(); it != this->_participates.end(); it++)
    {
        if (*it == user)
        {
            this->_participates.erase(it);
            break;
        }
    }
}

int Room::getId()
{
    return this->m_metadata.id;
}

bool Room::getIsActive()
{
    return this->m_metadata.isActive;
}

std::string Room::getName()
{
    return this->m_metadata.name;
}

int Room::getMaxPlayers()
{
    return this->m_metadata.maxPlayers;
}

void Room::setIsActive(bool isActive)
{
    this->m_metadata.isActive = isActive;
}

unsigned int Room::getQuestionsAmount()
{
    return this->m_metadata.numOfQuestionsInGame;
}

unsigned int Room::getQuestionTimeOut()
{
    return this->m_metadata.timePerQuestion;
}

std::vector<std::string> Room::getAllUsers()
{
    std::vector<std::string> usernames;
    for (LoggedUser user : this->_participates)
    {
        usernames.push_back(user.getUsername());
    }
    return usernames;
}
