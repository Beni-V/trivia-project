#include "Question.h"

std::string Question::getQuestion()
{
	return this->m_question;
}

std::vector<std::string> Question::getPossibleAnswers()
{
	return this->m_possibleAnswers;
}
