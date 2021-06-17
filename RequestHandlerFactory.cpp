#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory()
{
    this->m_database = new SqliteDatabase();
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    LoginRequestHandler* loginHandler = new LoginRequestHandler(*this);
    return loginHandler;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
    MenuRequestHandler* menuHandler = new MenuRequestHandler(*this, user);
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

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(Room& room, LoggedUser& user)
{
    return new RoomAdminRequestHandler(*this, room, user);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(Room& room, LoggedUser& user)
{
    return new RoomMemberRequestHandler(*this, room, user);
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser& user, std::vector<std::string> players)
{
    std::map<std::string, GameData> gameDataPlayers;
    std::vector<Question> questions = this->m_database->getQuestions();

    for (std::string player : players)
    {
        gameDataPlayers.insert({ player, GameData{questions[0], 0, 0, 0} });
    }

    Game game = Game(questions, gameDataPlayers);

    return new GameRequestHandler(*this, this->getGameManager(), game, user);
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return this->m_gameManager;
}
