#include "GameRequestHandler.h"

RequestResult GameRequestHandler::getQuestion(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse{ SUCCSESS_RESPONSE, this->m_game.getQuestionForUser(this->m_user.getUsername()).getQuestion()}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult GameRequestHandler::submitAnswer(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse{ SUCCSESS_RESPONSE, CORRECT_ANSWER_INDEX}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfoStruct.buffer);

	this->m_game.submitAnswer(this->m_user.getUsername(), this->m_game.getQuestionForUser(this->m_user.getUsername()).getPossibleAnswers()[request.answerId]);

	return requestResultStruct;
}

RequestResult GameRequestHandler::getGameResults(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetGameResultResponse{ SUCCSESS_RESPONSE}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult GameRequestHandler::leaveGame(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	this->m_game.removePlayer(this->m_user.getUsername());
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse{ SUCCSESS_RESPONSE }); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

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
