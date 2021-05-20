#include "RequestHandlerFactory.h"

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    LoginRequestHandler* loginHandler = new LoginRequestHandler(*this, getLoginManager());
    return loginHandler;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
    MenuRequestHandler* menuHandler = new MenuRequestHandler();
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
