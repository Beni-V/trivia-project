#include "SqliteDatabase.h"

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
        // create users table
        std::string sqlStatement = "CREATE TABLE USERS (USERNAME TEXT, PASSWORD TEXT, EMAIL TEXT);";
        sendSqlStatement(sqlStatement, NULL, NULL);

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

