#ifndef BOOKS_H
#define BOOKS_H

#include "sqlite3.h"
#include "why.h"

#define SUCCESS_MSG         ("Success")
#define ERROR_MSG           ("Error")
#define SCORE_DEFAULT       (0)
#define SCORE_MAX           (100)
#define STATUS_DEFAULT      (0)
#define LIMIT_DEFAULT       (10)
#define DB_NAME             ("books.db")
#define TABLE_NAME          ("books")
#define BUFFER_SIZE         (100)
#define USAGE_MSG           ("Usage:\nTo insert a record: ./books [name] [score] [status]\n\
                            To run a command: ./books [command] [argument]")
#define TEST_CMD_STR        ("--test")
#define ALL_CMD_STR         ("--all")
#define TOP_CMD_STR         ("--top")
#define UNFINISHED_CMD_STR  ("--reading")
#define UNREAD_CMD_STR      ("--unread")
#define FIND_CMD_STR        ("--find")

extern const char* COL_NAMES[];
extern const char* STATUS_STRINGS[];

typedef struct Book Book;
typedef enum STATUS STATUS;
typedef enum COMMAND COMMAND;
typedef enum COL_NAME COL_NAME;

struct Book
{
    char*   name;
    int     score;
    int     status;
};

enum STATUS
{
    STATUS_UNKONOWN = 100, UNREAD, READING, READ,
};

enum COL_NAME
{
    NAME_COL, SCORE_COL, STATUS_COL,
};

enum COMMAND
{
    UNKONOWN_CMD, INSERT_UPDATE_CMD, PRINT_ALL_CMD, TOP_CMD,
    UNFINISHED_CMD, UNREAD_CMD, FIND_CMD, TEST_CMD,
};

int     BookInit(Book* book, char* name, int score, STATUS status);
int     BookInitFromStrings(Book* book, int count, char** strings);
Book*   BookCreate(const char* name, int score, STATUS status);
void    BookDestroyWRAP(void* book);
int     InsertBook(sqlite3* db, const Book* book);
int     FindBook(sqlite3* db, const Book* book);
int     UpdateBook(sqlite3* db, const Book* book);
Deck*   SelectAllBooks(sqlite3* db);
Deck*   SelectTopRated(sqlite3* db, int count);
Deck*   SelectUnread(sqlite3* db, int count);
Deck*   SelectUnfinished(sqlite3* db, int count);
Deck*   SelectFind(sqlite3* db, const char* pattern);
void    PrintBookWRAP(const void* book);
void    PrintHeader();
void    PrintResultTable(const Deck* books);
COMMAND ParseCommand(int argc, char** argv);
int     UnknownCommand(sqlite3* db, int argc, char** argv);
int     InsertUpdateCommand(sqlite3* db, int argc, char** argv);
int     TopCommand(sqlite3* db, int argc, char** argv);
int     UnreadCommand(sqlite3* db, int argc, char** argv);
int     UnfinishedCommand(sqlite3* db, int argc, char** argv);
int     PrintAllCommand(sqlite3* db, int argc, char** argv);
int     TestCommand(sqlite3* db, int argc, char** argv);
int     FindCommand(sqlite3* db, int argc, char** argv);
int     QuitOnSuccess(sqlite3* db);
int     QuitOnError(sqlite3* db);

#endif