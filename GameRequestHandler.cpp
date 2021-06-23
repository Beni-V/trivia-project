#include "GameRequestHandler.h"

RequestResult GameRequestHandler::getQuestion(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	std::map<unsigned int, std::string> answers;

	for (int i = 0; i < this->m_game.getQuestionForUser(this->m_user.getUsername()).getPossibleAnswers().size(); i++)
	{
		answers.insert({ i, this->m_game.getQuestionForUser(this->m_user.getUsername()).getPossibleAnswers()[i] });
	}

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse{ SUCCSESS_RESPONSE, this->m_game.getQuestionForUser(this->m_user.getUsername()).getQuestion(), answers}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult GameRequestHandler::submitAnswer(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse{ SUCCSESS_RESPONSE, CORRECT_ANSWER_INDEX}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfoStruct.buffer);

	this->m_game.submitAnswer(this->m_user.getUsername(), request.answerId);

	this->m_gameManager.submitAnswer(this->m_game, this->m_user.getUsername(), request.answerId);

	if (request.answerId == CORRECT_ANSWER_INDEX)
	{
		this->m_handlerFactory.m_database->sendSqlStatement("UPDATE STATISTICS SET TOTAL_ANSWERS_AMOUNT = TOTAL_ANSWERS_AMOUNT + 1, CORRECT_ANSWERS_AMOUNT = CORRECT_ANSWERS_AMOUNT + 1 WHERE USERNAME = '" + this->m_user.getUsername()  + "';", NULL, NULL);
	}
	else
	{
		this->m_handlerFactory.m_database->sendSqlStatement("UPDATE STATISTICS SET TOTAL_ANSWERS_AMOUNT = TOTAL_ANSWERS_AMOUNT + 1 WHERE USERNAME = '" + this->m_user.getUsername() + "';", NULL, NULL);
	}

	return requestResultStruct;
}

RequestResult GameRequestHandler::getGameResults(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	for (Game game : this->m_gameManager.getGames())
	{
		if (game.getUserNames() == this->m_game.getUserNames())
		{
			requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetGameResultResponse{ SUCCSESS_RESPONSE, game.getPlayersResults() }); // fill buffer with serialized response
		}
	}

	requestResultStruct.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user); // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult GameRequestHandler::leaveGame(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	this->m_game.removePlayer(this->m_user.getUsername());
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse{ SUCCSESS_RESPONSE }); // fill buffer with serialized response
	requestResultStruct.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user); // fill newHandler with next handler

	return requestResultStruct;
}

bool GameRequestHandler::isRequestRelevant(RequestInfo requestInfoStruct)
{
	if (requestInfoStruct.requestId == GET_QUESTION_REQUEST ||
		requestInfoStruct.requestId == SUBMIT_ANSWER_REQUEST || 
		requestInfoStruct.requestId == GET_GAME_RESULT_REQUEST || 
		requestInfoStruct.requestId == LEAVE_GAME_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}

RequestResult GameRequestHandler::handleRequest(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	switch (requestInfoStruct.requestId)
	{
	case GET_QUESTION_REQUEST:
		requestResultStruct = this->getQuestion(requestInfoStruct);
		break;

	case SUBMIT_ANSWER_REQUEST:
		requestResultStruct = this->submitAnswer(requestInfoStruct);
		break;

	case GET_GAME_RESULT_REQUEST:
		requestResultStruct = this->getGameResults(requestInfoStruct);
		break;

	case LEAVE_GAME_REQUEST:
		requestResultStruct = this->leaveGame(requestInfoStruct);
		break;
	}

	return requestResultStruct;
}

GameRequestHandler::GameRequestHandler(RequestHandlerFactory& handlerFactory, GameManager& gameManager, Game& game, LoggedUser& user) : m_handlerFactory(handlerFactory), m_gameManager(gameManager), m_game(game), m_user(user)
{
}