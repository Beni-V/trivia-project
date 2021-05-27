#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RoomMemberRequestHandler : public IRequestHandler
{
private:
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult leaveRoom(RequestInfo requestInfoStruct);
	RequestResult getRoomState(RequestInfo requestInfoStruct);

public:
	RoomMemberRequestHandler(Room room, LoggedUser user, RoomManager roomManager, RequestHandlerFactory handlerFactory);
	bool isRequestRelevant(RequestInfo requestInfoStruct);
	RequestResult handleRequest(RequestInfo requestInfoStruct);

};
