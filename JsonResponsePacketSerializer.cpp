#include "JsonResponsePacketSerializer.h"

#define LOGIN_CODE 101
#define SIGNUP_CODE 102
#define ERROR_CODE 103
#define BYTE 8
#define SIZE_PART 4

using json = nlohmann::json;

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse LR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(LR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(LOGIN_CODE).to_string()).append(std::bitset<BYTE * SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SignupResponse SR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(SR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(SIGNUP_CODE).to_string()).append(std::bitset<BYTE * SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(ErrorResponse ER)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["message"] = ER.message;
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(ERROR_CODE).to_string()).append(std::bitset<BYTE * SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LogoutResponse LR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(LR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(SIGNUP_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse GRR)
{
	json jsonObject;
	std::string rooms;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(GRR.status);

	for (int i = 0; i < GRR.rooms.size(); i++)
	{
		rooms += GRR.rooms[i].name;
		if (i != GRR.rooms.size() - 1)
		{
			rooms += ", ";
		}
	}
	jsonObject["rooms"] = rooms;

	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(LOGIN_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse GPIRR)
{
	json jsonObject;
	std::string players;
	std::string responseMessage;

	//create a message in format JSON
	for (int i = 0; i < GPIRR.players.size(); i++)
	{
		players += GPIRR.players[i];
		if (i != GPIRR.players.size() - 1)
		{
			players += ", ";
		}
	}
	jsonObject["playersInRoom"] = players;

	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(LOGIN_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse GHSR)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse GPSR)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse JRR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(JRR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(LOGIN_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse CRR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(CRR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(LOGIN_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}
