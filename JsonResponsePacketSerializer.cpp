#include "JsonResponsePacketSerializer.h"

#define LOGIN_CODE 101
#define SIGNUP_CODE 102
#define ERROR_CODE 103
#define BYTE 8
#define SIZE_PART 4
#define CLOSE_ROOM_RESPONSE_CODE 104
#define START_GAME_RESPONSE_CODE 105
#define GET_ROOM_STATE_RESPONSE_CODE 106
#define LEAVE_ROOM_RESPONSE_CODE 107
#define GET_STATISTICS_RESPONSE_CODE 108
#define GET_HIGH_SCORES_RESPONSE_CODE 109
#define GET_GAME_RESULT_RESPONSE 110
#define SUBMIT_ANSWER_RESPONSE 111
#define GET_QUESTION_RESPONSE 112
#define LEAVE_GAME_RESPONSE 113

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
	std::map<int, std::string> rooms;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(GRR.status);

	for (int i = 0; i < GRR.rooms.size(); i++)
	{
		rooms.insert(std::pair<int, std::string>(GRR.rooms[i].id, GRR.rooms[i].name));
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
	std::vector<std::string> players;
	std::string responseMessage;

	//create a message in format JSON
	for (int i = 0; i < GPIRR.players.size(); i++)
	{
		players.push_back(GPIRR.players[i]);
	}
	jsonObject["players"] = players;

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
	json jsonObject;
	std::vector<std::string> statistics;
	std::string responseMessage;

	//create a message in format JSON
	for (int i = 0; i < GHSR.statistics.size(); i++)
	{
		statistics.push_back(GHSR.statistics[i]);
	}
	jsonObject["statistics"] = statistics;
	jsonObject["status"] = std::to_string(GHSR.status);


	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(GET_HIGH_SCORES_RESPONSE_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse GPSR)
{
	json jsonObject;
	std::vector<std::string> statistics;
	std::string responseMessage;

	//create a message in format JSON
	for (int i = 0; i < GPSR.statistics.size(); i++)
	{
		statistics.push_back(GPSR.statistics[i]);
	}
	jsonObject["statistics"] = statistics;
	jsonObject["status"] = std::to_string(GPSR.status);


	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(GET_STATISTICS_RESPONSE_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
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

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse CRR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(CRR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(CLOSE_ROOM_RESPONSE_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(StartGameResponse SGR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(SGR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(START_GAME_RESPONSE_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse GRR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(GRR.status);
	jsonObject["hasGameBegun"] = GRR.hasGameBegun;
	jsonObject["players"] = GRR.players;
	jsonObject["questionCount"] = std::to_string(GRR.questionCount);
	jsonObject["answerTimeout"] = std::to_string(GRR.answerTimeout);

	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(GET_ROOM_STATE_RESPONSE_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse LRR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(LRR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(LEAVE_ROOM_RESPONSE_CODE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetGameResultResponse GGRR)
{
	json jsonObject;
	std::string responseMessage;

	// convert results from response to format allowed by json
	std::map<std::string, std::vector<unsigned int>> results;
	for (PlayerResult pResult : GGRR.results)
	{
		results.insert({ pResult.username, { pResult.correctAnswerCount, pResult.wrongAnswerCount, pResult.averageAnswerTime } });
	}

	//create a message in format JSON
	jsonObject["status"] = std::to_string(GGRR.status);
	jsonObject["results"] = results;
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(GET_GAME_RESULT_RESPONSE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse SAR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(SAR.status);
	jsonObject["correctAnswerId"] = SAR.correctAnswerId;
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(SUBMIT_ANSWER_RESPONSE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse GQR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(GQR.status);
	jsonObject["question"] = GQR.question;
	jsonObject["answers"] = GQR.answers;
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(GET_QUESTION_RESPONSE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse LGR)
{
	json jsonObject;
	std::string responseMessage;

	//create a message in format JSON
	jsonObject["status"] = std::to_string(LGR.status);
	std::string message = jsonObject.dump();

	//append the message code and the message size to the response message
	responseMessage.append(std::bitset<BYTE>(LEAVE_GAME_RESPONSE).to_string()).append(std::bitset<BYTE* SIZE_PART>(message.size()).to_string());

	//append the message to the response message
	for (int i = 0; i < message.size(); i++)
	{
		responseMessage.append(std::bitset<BYTE>(message[i]).to_string());
	}
	return std::vector<unsigned char>(responseMessage.begin(), responseMessage.end());
}
