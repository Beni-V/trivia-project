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
        std::string sqlStatement = "CREATE TABLE USERS (USERNAME TEXT, PASSWORD TEXT, EMAIL TEXT); ";
        sendSqlStatement(sqlStatement, nullptr, nullptr);

    }
}

/*
input: sqlStatement that should be executed, callback function (may be nullptr), data for output from callback (also may be nullptr)
output: null
function will execute sql statement and return result from callback to the third parameter if second and third parameters are not null
*/
void SqliteDatabase::sendSqlStatement(std::string sqlStatement, int(*callback)(void*, int, char**, char**), void* data)
{
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, sqlStatement.c_str(), callback, data, &errMessage); // execute statement
    if (res != SQLITE_OK)
        std::cout << "Trouble with sending sqlstatement: " << sqlStatement << std::endl;
}

