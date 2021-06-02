#include "RoomAdminRequestHandler.h"

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo requestInfoStrust)
{
	RequestResult requestResultStruct;

	this->m_roomManager.deleteRoom(this->m_room.getId());

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse{SUCCSESS_RESPONSE}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo requestInfoStrust)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ SUCCSESS_RESPONSE }); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo requestInfoStrust)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse{ SUCCSESS_RESPONSE, this->m_room.getIsActive(), this->m_room.getAllUsers(), this->m_room.getQuestionsAmount(), this->m_room.getQuestionTimeOut()}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

RoomAdminRequestHandler::RoomAdminRequestHandler(RoomManager& roomManager, Room& room, LoggedUser user, RequestHandlerFactory RHF) :m_roomManager(roomManager), m_room(room), m_user(user), m_handlerFactory(RHF)
{
}

// will check if request id recieved from user is relevant
bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo requestInfoStruct)
{
	// return true if request id is relevant and false if its not
	if (requestInfoStruct.requestId == CLOSE_ROOM_REQUEST ||
		requestInfoStruct.requestId == START_GAME_REQUEST ||
		requestInfoStruct.requestId == GET_ROOM_STATE_REQUEST ||
		requestInfoStruct.requestId == LEAVE_ROOM_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// will call the needed handler for each possible request for RoomAdmin state
RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	switch (requestInfoStruct.requestId)
	{
	case CLOSE_ROOM_REQUEST:
		requestResultStruct = this->closeRoom(requestInfoStruct);
		break;

	case START_GAME_REQUEST:
		requestResultStruct = this->startGame(requestInfoStruct);
		break;

	case GET_ROOM_STATE_REQUEST:
		requestResultStruct = this->getRoomState(requestInfoStruct);
		break;
	}

	return requestResultStruct;
}
