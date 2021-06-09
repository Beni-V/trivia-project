#pragma once
#include <string>
#include <vector>

#define CORRECT_ANSWER_INDEX 0 

class Question
{
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;

public:
	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	std::string getCorrectAnswer();
};