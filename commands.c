#include "books.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

COMMAND ParseCommand(int argc, char** argv)
{
    if (argc == 1)
        return UNKONOWN_CMD;

    if (strcmp(argv[1], ALL_CMD_STR) == 0)
        return PRINT_ALL_CMD;
    if (strcmp(argv[1], TOP_CMD_STR) == 0)
        return TOP_CMD;
    if (strcmp(argv[1], UNREAD_CMD_STR) == 0)
        return UNREAD_CMD;
    if (strcmp(argv[1], UNFINISHED_CMD_STR) == 0)
        return UNFINISHED_CMD;
    if (strcmp(argv[1], FIND_CMD_STR) == 0)
        return FIND_CMD;
    if (strcmp(argv[1], TEST_CMD_STR) == 0)
        return TEST_CMD;

    return INSERT_UPDATE_CMD;
}

int UnknownCommand(sqlite3* db, int argc, char** argv)
{
    (void)argc;
    (void)argv;

    fprintf(stderr, "%s\n", USAGE_MSG);
    return QuitOnError(db);
}

int InsertUpdateCommand(sqlite3* db, int argc, char** argv)
{
    int     status;
    Book    book;

    status = BookInitFromStrings(&book, argc - 1, argv + 1);
    if (status == WHY_ERROR)
        return QuitOnError(db);

    if (FindBook(db, &book))
        return UpdateBook(db, &book) ? QuitOnError(db) : QuitOnSuccess(db);

    return InsertBook(db, &book) ? QuitOnError(db) : QuitOnSuccess(db);
}

static int _select_with_count_command(sqlite3* db, int argc, char** argv, Deck* (*command)(sqlite3* , int))
{
    Deck*   books;
    Int     count;

    if (argc == 2)
        books = command(db, LIMIT_DEFAULT);
    else if (argc == 3)
    {
        if (ParseInt(&count, argv[2]) <= 0)
            return QuitOnError(db);
        if (count <= 0)
            return QuitOnError(db);
        
        books = command(db, count);
    }
    else return QuitOnError(db);

    PrintResultTable(books);
    DeckDestroy(books);
    
    return QuitOnSuccess(db);
}

int TopCommand(sqlite3* db, int argc, char** argv)
{
    return _select_with_count_command(db, argc, argv, SelectTopRated);
}

int UnfinishedCommand(sqlite3* db, int argc, char** argv)
{
    return _select_with_count_command(db, argc, argv, SelectUnfinished);
}

int UnreadCommand(sqlite3* db, int argc, char** argv)
{
    return _select_with_count_command(db, argc, argv, SelectUnread);
}

int PrintAllCommand(sqlite3* db, int argc, char** argv)
{
    Deck* books;

    (void)argc;
    (void)argv;

    books = SelectAllBooks(db);
    PrintResultTable(books);
    DeckDestroy(books);

    return QuitOnSuccess(db);
}

int TestCommand(sqlite3* db, int argc, char** argv)
{
    return PrintAllCommand(db, argc, argv);
}

int FindCommand(sqlite3* db, int argc, char** argv)
{
    char* pattern;
    Deck* books;

    if (argc != 3)
        return QuitOnError(db);
    
    pattern = argv[2];
    books = SelectFind(db, pattern);
    PrintResultTable(books);
    DeckDestroy(books);

    return QuitOnSuccess(db);
}