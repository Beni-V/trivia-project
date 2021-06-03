#include "RequestHandlerFactory.h"

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    LoginRequestHandler* loginHandler = new LoginRequestHandler(*this, getLoginManager());
    return loginHandler;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(RoomManager& roomManager, LoggedUser user)
{
    MenuRequestHandler* menuHandler = new MenuRequestHandler(roomManager, this->getStatisticsManager(), *this, user);
    return menuHandler;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return this->m_StatisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return this->m_roomManager;
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(Room& room, LoggedUser user)
{
    return new RoomAdminRequestHandler(this->m_roomManager, room, user, *this);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(Room& room, LoggedUser user, RoomManager& roomManager)
{
    return new RoomMemberRequestHandler(room, user, roomManager, *this);
}

