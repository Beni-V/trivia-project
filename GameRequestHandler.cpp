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
