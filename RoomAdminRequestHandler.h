#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult closeRoom(RequestInfo requestInfoStrust);
	RequestResult startGame(RequestInfo requestInfoStrust);
	RequestResult getRoomState(RequestInfo requestInfoStrust);

public:
	bool isRequestRelevant(RequestInfo requestInfoStruct);
	RequestResult handleRequest(RequestInfo requestInfoStruct);

};