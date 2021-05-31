#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory, LoginManager& loginManager)
	: m_handlerFactory(handlerFactory), m_loginManager(loginManager)
{}

// function will return true if the code message is relevant and false if its not
bool LoginRequestHandler::isRequestRelevant(RequestInfo requestInfoStruct)
{
	if (requestInfoStruct.requestId == LOGIN_REQUEST)
	{
		return true;
	}
	else if (requestInfoStruct.requestId == SIGNUP_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	if (requestInfoStruct.requestId == LOGIN_REQUEST)
	{
		requestResultStruct = this->login(requestInfoStruct);
	}
	else if (requestInfoStruct.requestId == SIGNUP_REQUEST)
	{
		requestResultStruct = this->signup(requestInfoStruct);
	}	

	return requestResultStruct;
}

RequestResult LoginRequestHandler::login(RequestInfo info)
{
	RequestResult requestResultStruct;

	// if the login succeed
	if (this->m_loginManager.login(JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer).username, JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer).password))
	{
		LoginResponse loginResponse;
		loginResponse.status = 1;

		// fill RequestResult struct buffer with the server message
		requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(loginResponse);
		// continue to the next step, Menu
		requestResultStruct.newHandler = this->m_handlerFactory.createMenuRequestHandler();
	}
	else
	{
		ErrorResponse errorResponse;
		errorResponse.message = "ERROR";

		// fill RequestResult struct buffer with the server message
		requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		// try again to login
		requestResultStruct.newHandler = this->m_handlerFactory.createLoginRequestHandler();
	}
	return requestResultStruct;
}

RequestResult LoginRequestHandler::signup(RequestInfo info)
{
	RequestResult requestResultStruct;

	// if the signup succeed
	if (this->m_loginManager.signup(JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer).username, JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer).password, JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer).email))
	{
		SignupResponse signupResponse;
		signupResponse.status = 1;

		// fill RequestResult struct buffer with the server message
		requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(signupResponse);
		// continue to the next step, Menu
		requestResultStruct.newHandler = this->m_handlerFactory.createMenuRequestHandler();
	}
	else
	{
		ErrorResponse errorResponse;
		errorResponse.message = "User with this name is already exist.";

		// fill RequestResult struct buffer with the server message
		requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		// try again to signup
		requestResultStruct.newHandler = this->m_handlerFactory.createLoginRequestHandler();
	}
	return requestResultStruct;
}
