#include "JsonRequestPacketDeserializer.h"

#define HEADERS_LENGTH 40 // the meaning is length of message code and message length in bits
#define BYTE 8

//This methods will get a buffer of bits wich is serialized message and will deserialize this message into struct


LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<unsigned char> Buffer)
{
	std::string serializedJsonMessage(Buffer.begin() + HEADERS_LENGTH, Buffer.end()); // the string will store only the json message part in bits
	std::stringstream sstream(serializedJsonMessage);
	std::string deserializedJsonMessage;
	json deserializedJsonObject;
	LoginRequest result;

	// deserialize the json message into deserializedJsonMessage
	while (sstream.good())
	{
		std::bitset<BYTE> bits;
		sstream >> bits;
		char messageChar = char(bits.to_ulong());
		deserializedJsonMessage += messageChar;
	}

	deserializedJsonObject = json::parse(deserializedJsonMessage); // parse json string into json object

	// built the struct that will be returned
	result.username = deserializedJsonObject["username"];
	result.password = deserializedJsonObject["password"];

	return result;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(std::vector<unsigned char> Buffer)
{
	std::string serializedJsonMessage(Buffer.begin() + HEADERS_LENGTH, Buffer.end()); // the string will store only the json message part in bits
	std::stringstream sstream(serializedJsonMessage);
	std::string deserializedJsonMessage;
	json deserializedJsonObject;
	SignupRequest result;

	// deserialize the json message into deserializedJsonMessage
	while (sstream.good())
	{
		std::bitset<BYTE> bits;
		sstream >> bits;
		char messageChar = char(bits.to_ulong());
		deserializedJsonMessage += messageChar;
	}

	deserializedJsonObject = json::parse(deserializedJsonMessage); // parse json string into json object

	// built the struct that will be returned
	result.username = deserializedJsonObject["username"];
	result.password = deserializedJsonObject["password"];
	result.email = deserializedJsonObject["email"];

	return result;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(std::vector<unsigned char> Buffer)
{
	std::string serializedJsonMessage(Buffer.begin() + HEADERS_LENGTH, Buffer.end()); // the string will store only the json message part in bits
	std::stringstream sstream(serializedJsonMessage);
	std::string deserializedJsonMessage;
	json deserializedJsonObject;
	GetPlayersInRoomRequest result;

	// deserialize the json message into deserializedJsonMessage
	while (sstream.good())
	{
		std::bitset<BYTE> bits;
		sstream >> bits;
		char messageChar = char(bits.to_ulong());
		deserializedJsonMessage += messageChar;
	}

	deserializedJsonObject = json::parse(deserializedJsonMessage); // parse json string into json object

	// built the struct that will be returned
	result.roomId = deserializedJsonObject["roomId"];

	return result;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(std::vector<unsigned char> Buffer)
{
	std::string serializedJsonMessage(Buffer.begin() + HEADERS_LENGTH, Buffer.end()); // the string will store only the json message part in bits
	std::stringstream sstream(serializedJsonMessage);
	std::string deserializedJsonMessage;
	json deserializedJsonObject;
	JoinRoomRequest result;

	// deserialize the json message into deserializedJsonMessage
	while (sstream.good())
	{
		std::bitset<BYTE> bits;
		sstream >> bits;
		char messageChar = char(bits.to_ulong());
		deserializedJsonMessage += messageChar;
	}

	deserializedJsonObject = json::parse(deserializedJsonMessage); // parse json string into json object

	// built the struct that will be returned
	result.roomId = deserializedJsonObject["roomId"];

	return result;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::vector<unsigned char> Buffer)
{
	std::string serializedJsonMessage(Buffer.begin() + HEADERS_LENGTH, Buffer.end()); // the string will store only the json message part in bits
	std::stringstream sstream(serializedJsonMessage);
	std::string deserializedJsonMessage;
	json deserializedJsonObject;
	CreateRoomRequest result;

	// deserialize the json message into deserializedJsonMessage
	while (sstream.good())
	{
		std::bitset<BYTE> bits;
		sstream >> bits;
		char messageChar = char(bits.to_ulong());
		deserializedJsonMessage += messageChar;
	}

	deserializedJsonObject = json::parse(deserializedJsonMessage); // parse json string into json object

	// built the struct that will be returned
	result.answerTimeout = deserializedJsonObject["answerTimeout"];
	result.maxUsers = deserializedJsonObject["maxUsers"];
	result.questionCount = deserializedJsonObject["questionCount"];
	result.roomName = deserializedJsonObject["roomName"];

	return result;
}
