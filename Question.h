#pragma once
#include <string>
#include <vector>

#define CORRECT_ANSWER_INDEX 4

class Question
{
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;

public:
	Question(std::string question, std::vector<std::string> possibleAnswers);
	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	std::string getCorrectAnswer();
	bool operator==(const Question& other);
};