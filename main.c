#include "books.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    sqlite3*    db;
    COMMAND     command;

    WhyStart();
    db = NULL;

    if (sqlite3_open(DB_NAME, &db))
        return QuitOnError(db);
    
    command = ParseCommand(argc, argv);

    if (command == UNKONOWN_CMD)
        return UnknownCommand(db, argc, argv);
    if (command == INSERT_UPDATE_CMD)
        return InsertUpdateCommand(db, argc, argv);
    if (command == PRINT_ALL_CMD)
        return PrintAllCommand(db, argc, argv);
    if (command == TOP_CMD)
        return TopCommand(db, argc, argv);
    if (command == UNFINISHED_CMD)
        return UnfinishedCommand(db, argc, argv);
    if (command == UNREAD_CMD)
        return UnreadCommand(db, argc, argv);
    if (command == FIND_CMD)
        return FindCommand(db, argc, argv);
    if (command == TEST_CMD)
        return TestCommand(db, argc, argv);

    return QuitOnError(db);
}