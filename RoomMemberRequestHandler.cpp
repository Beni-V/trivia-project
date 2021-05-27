#include "RoomMemberRequestHandler.h"

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	this->m_room.removeUser(this->m_user); // remove user from room
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{SUCCSESS_RESPONSE}); // fill buffer with serialized response
	requestResultStruct.newHandler = this->m_handlerFactory.createMenuRequestHandler(); // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse{ SUCCSESS_RESPONSE, this->m_room.getIsActive(), this->m_room.getAllUsers(), this->m_room.getQuestionsAmount(), this->m_room.getQuestionTimeOut() }); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
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
