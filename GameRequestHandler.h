#pragma once
#include "Game.h"
#include "LoggedUser.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"

class GameRequestHandler : public IRequestHandler
{
private:
	Game m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult getQuestion(RequestInfo requestInfoStruct);
	RequestResult submitAnswer(RequestInfo requestInfoStruct);
	RequestResult getGameResults(RequestInfo requestInfoStruct);
	RequestResult leaveGame(RequestInfo requestInfoStruct);

public:
	GameRequestHandler(RequestHandlerFactory& handlerFactory, GameManager& gameManager, Game& game, LoggedUser& user);
	bool isRequestRelevant(RequestInfo requestInfoStruct);
	RequestResult handleRequest(RequestInfo requestInfoStruct);
};
