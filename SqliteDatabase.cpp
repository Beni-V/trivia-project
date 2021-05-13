#include "SqliteDatabase.h"

#define QUESTION_INDEX 0
#define FIRST_ANSWER_INDEX 1
#define SECOND_ANSWER_INDEX 2
#define THIRD_ANSWER_INDEX 3
#define RIGHT_ANSWER_INDEX 4

SqliteDatabase::SqliteDatabase()
{
    std::string dbFileName = "MyDB.sqlite";

    int doesFileExist = _access(dbFileName.c_str(), 0); // check if db is already exist (before trying to create)

    int res = sqlite3_open(dbFileName.c_str(), &db); // open the db
    if (res != SQLITE_OK) // if failed to open db
    {
        db = nullptr;
        std::cout << "Failed to open DB" << std::endl;
    }
    else // if not failed
    {
        std::cout << "DB opened succsessfully!\n";
    }


    if (doesFileExist == 0) // if DB elready exist
    {
        std::cout << "DB already exist\n";
    }

    else // if not, create the tables
    {
        // this vector will store lists with the questions (list is: Question and the answers)
        // first element in list is the question, 2-4 is the wrong answers, 4 is the right answer
        std::vector<std::vector<std::string>> questions{
            {"What is the capital of USA?", "New York", "Los Angeles", "Miami", "Washington"},
            {"How many bits are there in one byte?", "4", "10", "16", "8"},
            {"Who created the Linux Operating System?", "Elon Musk", "Bill Gates", "Bibi Nethanyahu", "Linus Torvalds"},
            {"What is the worst programming language on the planet earth?", "Python", "C++", "Assembly", "JavaScript"},
            {"Who is the richest person in the world?", "Bill Gates", "Elon Musk", "Jeff Bezos", "Vladimir Putin"},
            {"Which chess piece is the most valuable?", "Rook", "Bishop", "Queen", "King"},
            {"What is Debugging?", "Insect elimination process", "Kind of sport", "Musical genre", "Being the detective in a crime movie where you are also the murderer"},
            {"What Americans do at least a hundred times a day?", "Blinking", "Open up a smartphone", "Steps", "Open up the refrigerator"},
            {"What is the capital of Israel?", "Haifa", "Rishon-Lezion", "Kfar-Saba", "Jerusalem"},
            {"Which of these tools is the most powerful for collecting data?", "Nmap", "WPScan", "Wireshark", "Facebook"}
        };

        // create users table
        std::string sqlStatement = "CREATE TABLE USERS (USERNAME TEXT, PASSWORD TEXT, EMAIL TEXT);";
        sendSqlStatement(sqlStatement, NULL, NULL);

        // create questions table
        sqlStatement = "CREATE TABLE QUESTIONS (ID INTEGER NOT NULL PRIMARY KEY, QUESTION TEXT, FIRST_WRONG_ANSWER TEXT, SECOND_WRONG_ANSWER TEXT, THIRD_WRONG_ANSWER TEXT, FOURTH_RGIHT_ANSWER TEXT);";
        sendSqlStatement(sqlStatement, NULL, NULL);

        // create statistics table
        sqlStatement = "CREATE TABLE STATISTICS (USERNAME TEXT, AVG_ANSWER_TIME FLOAT, CORRECT_ANSWERS_AMOUNT INTEGER, TOTAL_ANSWERS_AMOUNT INTEGER, GAMES_DONE_AMOUNT INTEGER);";
        sendSqlStatement(sqlStatement, NULL, NULL);

        // add the questions to questions table
        for (std::vector<std::vector<std::string>>::iterator it = questions.begin(); it != questions.end(); it++)
        {
            sqlStatement = "INSERT INTO QUESTIONS (QUESTION, FIRST_WRONG_ANSWER, SECOND_WRONG_ANSWER, THIRD_WRONG_ANSWER, FOURTH_RGIHT_ANSWER) VALUES ('" + it->at(QUESTION_INDEX) + "', '" + it->at(FIRST_ANSWER_INDEX) + "', '" + it->at(SECOND_ANSWER_INDEX) + "', '" + it->at(THIRD_ANSWER_INDEX) + "', '" + it->at(RIGHT_ANSWER_INDEX) + "');";
            sendSqlStatement(sqlStatement, NULL, NULL);
        }
    }
}

/* this function will return answer from db in case that is a single number
will be used for checking if username with a specific name exist*/
int callBackGetInt(void* data, int argc, char** argv, char** azColName)
{
    int* result = (int*)data; // that will be the output for function
    *result = std::atoi(argv[0]); // take the only collum in answer

    return 0;
}

int callBackGetFloat(void* data, int argc, char** argv, char** azColName)
{
    float* result = (float*)data; // that will be the output for function
    *result = std::atoi(argv[0]); // take the only collum in answer

    return 0;
}

int callBackGetQuestions(void* data, int argc, char** argv, char** azColName)
{
    std::vector<std::vector<std::string>>* result = (std::vector<std::vector<std::string>>*) data;

    // added one to defines because the first element is the id in the questions table, its not needed when vector allows me to get question by index
    result->push_back({ argv[QUESTION_INDEX + 1], argv[FIRST_ANSWER_INDEX + 1], argv[SECOND_ANSWER_INDEX + 1], argv[THIRD_ANSWER_INDEX + 1], argv[RIGHT_ANSWER_INDEX + 1]});

    return 0;
}

// will return true if there is user with given username in the db and false if there is no user
bool SqliteDatabase::doesUserExist(std::string userName)
{
    int doesUserExist = 0;

    std::string sqlStatement = "SELECT COUNT(*) FROM USERS WHERE USERNAME == '" + userName + "';";
    sendSqlStatement(sqlStatement, callBackGetInt, &doesUserExist);

    return doesUserExist;
}

// will return true if there is user with the given username and password and false if there is no user
bool SqliteDatabase::doesPasswordMatch(std::string userName, std::string password)
{
    int doesUserExist = 0;

    std::string sqlStatement = "SELECT COUNT(*) FROM USERS WHERE USERNAME == '" + userName + "' AND PASSWORD == '" + password + "';";
    sendSqlStatement(sqlStatement, callBackGetInt, &doesUserExist);

    return doesUserExist;
}

void SqliteDatabase::addNewUser(std::string userName, std::string password, std::string email)
{
    std::string sqlStatement = "INSERT INTO USERS(USERNAME, PASSWORD, EMAIL) VALUES('" + userName + "', '" + password + "', '" + email + "');";
    sendSqlStatement(sqlStatement, NULL, NULL);
}

void SqliteDatabase::sendSqlStatement(std::string sqlStatement, int(*callback)(void*, int, char**, char**), void* data)
{
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, sqlStatement.c_str(), callback, data, &errMessage); // execute statement
    if (res != SQLITE_OK)
        std::cout << "Trouble with sending sqlstatement: " << sqlStatement << std::endl;
}

std::vector<std::vector<std::string>> SqliteDatabase::getQuestions()
{
    std::vector<std::vector<std::string>> questions;

    std::string sqlStatement = "SELECT * FROM QUESTIONS;";
    sendSqlStatement(sqlStatement, callBackGetQuestions, &questions);

    return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(std::string userName)
{
    float result;

    std::string sqlStatement = "SELECT AVG_ANSWER_TIME FROM STATISTICS WHERE USERNAME = '" + userName + "';";
    sendSqlStatement(sqlStatement, callBackGetFloat, &result);

    return result;
}

int SqliteDatabase::getNumOfCorrectAnswers(std::string userName)
{
    int result;
    
    std::string sqlStatement = "SELECT CORRECT_ANSWERS_AMOUNT FROM STATISTICS WHERE USERNAME = '" + userName + "';";
    sendSqlStatement(sqlStatement, callBackGetInt, &result);

    return result;
}

int SqliteDatabase::getNumOfTotalAnswers(std::string userName)
{
    int result;

    std::string sqlStatement = "SELECT TOTAL_ANSWERS_AMOUNT FROM STATISTICS WHERE USERNAME = '" + userName + "';";
    sendSqlStatement(sqlStatement, callBackGetInt, &result);

    return result;
}

int SqliteDatabase::getNumOfPlayerGames(std::string userName)
{
    int result;

    std::string sqlStatement = "SELECT GAMES_DONE_AMOUNT FROM STATISTICS WHERE USERNAME = '" + userName + "';";
    sendSqlStatement(sqlStatement, callBackGetInt, &result);

    return result;
}

