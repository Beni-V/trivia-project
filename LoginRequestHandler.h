#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& factory);

	bool isRequestRelevant(RequestInfo info) override;
	RequestResult handleRequest(RequestInfo info) override;

private:
	RequestResult login(RequestInfo info);
	RequestResult signup(RequestInfo info);

	RequestHandlerFactory& m_handlerFactory;
	LoginManager& m_loginManager;
};
