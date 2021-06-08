#include "RoomMemberRequestHandler.h"

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	this->m_roomManager.removeUserFromRoom(this->m_room.getId(), this->m_user);
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{SUCCSESS_RESPONSE}); // fill buffer with serialized response
	requestResultStruct.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user); // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	std::map<unsigned int, Room> rooms = this->m_roomManager.getRooms();

	if (rooms.find((unsigned int)this->m_room.getId()) != rooms.end())
	{
		requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse{ SUCCSESS_RESPONSE, this->m_room.getIsActive(), this->m_roomManager.getRooms()[this->m_room.getId()].getAllUsers(), this->m_room.getQuestionsAmount(), this->m_room.getQuestionTimeOut() }); // fill buffer with serialized response
		requestResultStruct.newHandler = this; // fill newHandler with next handler
	}
	else
	{
		requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{"Room Closed"}); // fill buffer with serialized response
		requestResultStruct.newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user); // fill newHandler with next handler
	}

	return requestResultStruct;
}

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& handlerFactory, Room& room, LoggedUser& user) : m_room(room), m_user(user), m_roomManager(handlerFactory.getRoomManager()), m_handlerFactory(handlerFactory)
{
}

// will check if request id is relevant for current state
bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo requestInfoStruct)
{
	if (requestInfoStruct.requestId == LEAVE_ROOM_REQUEST || requestInfoStruct.requestId == GET_ROOM_STATE_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// will call for the right handler of this state
RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	switch (requestInfoStruct.requestId)
	{
	case LEAVE_ROOM_REQUEST:
		requestResultStruct = this->leaveRoom(requestInfoStruct);
		break;

	case GET_ROOM_STATE_REQUEST:
		requestResultStruct = this->getRoomState(requestInfoStruct);
		break;
	}

	return requestResultStruct;
}
