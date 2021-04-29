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
	jsonObject["status"] = ER.message;
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
