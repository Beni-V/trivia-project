#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user): m_roomManager(handlerFactory.getRoomManager()), m_statisticsManager(handlerFactory.getStatisticsManager()), m_handlerFactory(handlerFactory), m_user(user)
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo info)
{
	// if request id is equal to one of the defined requests id, return true.
	if (info.requestId == CREATE_ROOM_REQUEST ||
		info.requestId == GET_ROOMS_REQUEST ||
		info.requestId == GET_PLAYERS_IN_ROOM_REQUEST ||
		info.requestId == JOIN_ROOM_REQUEST ||
		info.requestId == GET_STATISTICS_REQUEST ||
		info.requestId == LOGOUT_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo info)
{
	RequestResult requestResultStruct;

	switch (info.requestId)
	{
	case CREATE_ROOM_REQUEST:
		requestResultStruct = this->createRoom(info);
		break;

	case GET_ROOMS_REQUEST:
		requestResultStruct = this->getRooms(info);
		break;

	case GET_PLAYERS_IN_ROOM_REQUEST:
		requestResultStruct = this->getPlayersInRoom(info);
		break;

	case JOIN_ROOM_REQUEST:
		requestResultStruct = this->joinRoom(info);
		break;

	case GET_STATISTICS_REQUEST:
		requestResultStruct = this->getPersonalStats(info);
		break;

	case LOGOUT_REQUEST:
		requestResultStruct = this->signout(info);
		break;
	}

	return requestResultStruct;
}

RequestResult MenuRequestHandler::signout(RequestInfo info)
{
	RequestResult requestResultStruct;

	this->m_handlerFactory.getLoginManager().logout(this->m_user.getUsername()); // logout user
	requestResultStruct.newHandler = this->m_handlerFactory.createLoginRequestHandler(); // fill newHandler with next handler (which is login)
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(LogoutResponse{ SUCCSESS_RESPONSE }); // fill Buffer with serialized logout response
	return requestResultStruct;
}

RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
	RequestResult requestResultStruct;

	std::vector<RoomData> roomsData;

	std::map<unsigned int, Room> rooms = this->m_handlerFactory.getRoomManager().getRooms();

	for (std::pair<unsigned int, Room> room : rooms)
	{
		roomsData.push_back(RoomData{ room.first, room.second.getName(), (unsigned int)room.second.getMaxPlayers(), (unsigned int)room.second.getQuestionsAmount(), (unsigned int)room.second.getQuestionTimeOut(), room.second.getIsActive() });
	}

	requestResultStruct.newHandler = this; // fill newHandler with next handler
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse{ SUCCSESS_RESPONSE, roomsData}); // fill Buffer with serialized get rooms response
	return requestResultStruct;
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
	RequestResult requestResultStruct;

	GetPlayersInRoomResponse response = {this->m_handlerFactory.getRoomManager().getRooms()[JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(info.buffer).roomId].getAllUsers()}; // create response with users in room
	requestResultStruct.newHandler = this; // fill newHandler with next handler
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(response); // fill buffer with serialized response

	return requestResultStruct;
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
	RequestResult requestResultStruct;

	GetPersonalStatsResponse response = { SUCCSESS_RESPONSE, this->m_statisticsManager.getUserStatistics(this->m_user.getUsername()) }; // cerate response with user stats
	requestResultStruct.newHandler = this; // fill newHandler with next handler
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(response); // fill buffer with serialized response

	return requestResultStruct;
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
	RequestResult requestResultStruct;

	GetHighScoreResponse response = { SUCCSESS_RESPONSE, this->m_statisticsManager.getHighScore() }; // create response with top scored users
	requestResultStruct.newHandler = this; // fill newHandler with next handler
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(response); // fill buffer with serialized response

	return requestResultStruct;
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
	RequestResult requestResultStruct;

	JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer); // deserialize request
	this->m_roomManager.addUserToRoom(request.roomId, this->m_user);

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse{ SUCCSESS_RESPONSE }); // fill buffer with serialized response
	requestResultStruct.newHandler = this->m_handlerFactory.createRoomMemberRequestHandler(this->m_handlerFactory.getRoomManager().getRooms()[request.roomId], this->m_user); // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
	RequestResult requestResultStruct;

	CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer); // deserialize request
	this->m_handlerFactory.getRoomManager().createRoom(this->m_user, RoomData{(unsigned int)(this->m_roomManager.getRooms().size() + 1), request.roomName, request.maxUsers, request.questionCount, request.answerTimeout, false}); // create room

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse{ SUCCSESS_RESPONSE });// fill buffer with serialized response

	requestResultStruct.newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(this->m_handlerFactory.getRoomManager().getRooms()[(unsigned int)(this->m_roomManager.getRooms().size())], this->m_user); // fill newHandler with next handler

	return requestResultStruct;
}


