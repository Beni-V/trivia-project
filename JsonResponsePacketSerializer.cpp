#include "JsonResponsePacketSerializer.h"

#define LOGIN_CODE 101
#define SIGNUP_CODE 102
#define ERROR_CODE 103
#define BYTE 8
#define SIZE_PART 4

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse LR)
{
	std::string responseMessage;
	//create a message in format JSON
	std::string message = "{status:" + std::to_string(LR.status) + "}";

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
	std::string responseMessage;
	//create a message in format JSON
	std::string message = "{status:" + std::to_string(SR.status) + "}";

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
	std::string responseMessage;
	//create a message in format JSON
	std::string message = "{message:" + ER.message + "}";

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(ERROR_CODE).to_string()).append(std::bitset<BYTE * SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}
