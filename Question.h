#pragma once
#include <string>
#include <vector>

class Question
{
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;

public:
	std::string getQuestions();
	std::vector<std::string> getPossibleAnswers();
	std::string getCorrectAnswer();
};