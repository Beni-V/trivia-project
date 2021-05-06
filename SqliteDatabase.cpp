#include "SqliteDatabase.h"

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