#include "books.h"

#include <stdio.h>

#define INSERT_SQL_FORMAT               ("insert into %s values('%s', %d, %d)")
#define UPDATE_SQL_FORMAT               ("update %s set %s = %d, %s = %d where %s == '%s'")
#define FIND_SQL_FORMAT                 ("select 1 from %s where %s == '%s'")
#define SELECT_ALL_SQL_FORMAT           ("select * from %s limit %d")
#define SELECT_BY_INT_FIELD_SQL_FORMAT  ("select * from %s where %s == %d limit %d")
#define SELECT_ORDER_SQL_FORMAT         ("select * from %s order by %s limit %d")
#define SELECT_ORDER_WHERE_SQL_FORMAT   ("select * from %s where %s == %d order by %s desc limit %d")
#define SELECT_FIND_SQL_FORMAT          ("select * from %s where %s like '%%""%s""%%'")

static char* _prep_query(Uint size)
{
    char* query;

    query = MemZero(size);
    WhySavePtr(&query);

    return query;
}

char* get_insert_query(const Book* book)
{
    char* query;

    query = _prep_query(BUFFER_SIZE);
    sprintf(query, INSERT_SQL_FORMAT, TABLE_NAME, book->name, book->score, book->status);

    return query;
}

char* get_find_query(const Book* book)
{
    char* query;

    query = _prep_query(BUFFER_SIZE);
    sprintf(query, FIND_SQL_FORMAT, TABLE_NAME, COL_NAMES[NAME_COL], book->name);

    return query;
}

char* get_update_query(const Book* book)
{
    char* query;

    query = _prep_query(BUFFER_SIZE);
    sprintf(query, UPDATE_SQL_FORMAT, TABLE_NAME,
            COL_NAMES[SCORE_COL], book->score, COL_NAMES[STATUS_COL],
            book->status, COL_NAMES[NAME_COL], book->name);

    return query;
}

char* get_select_all_query()
{
    char* query;

    query = _prep_query(BUFFER_SIZE);
    sprintf(query, SELECT_ALL_SQL_FORMAT, TABLE_NAME, -1);

    return query;
}

char* get_select_query(const char* col_name, int value, int limit)
{
    char* query;

    query = _prep_query(BUFFER_SIZE);
    sprintf(query, SELECT_BY_INT_FIELD_SQL_FORMAT, TABLE_NAME, col_name, value, limit);

    return query;
}

char* get_select_ordered_query(const char* col_name, int limit)
{
    char* query;

    query = _prep_query(BUFFER_SIZE);
    sprintf(query, SELECT_ORDER_SQL_FORMAT, TABLE_NAME, col_name, limit);

    return query;
}

char* get_select_ordered_with_condition_query(const char* col_name, int value, const char* order_by, int limit)
{
    char* query;

    query = _prep_query(BUFFER_SIZE);
    sprintf(query, SELECT_ORDER_WHERE_SQL_FORMAT, TABLE_NAME, col_name, value, order_by, limit);

    return query;
}

char* get_select_find_query(const char* col_name, const char* pattern)
{
    char* query;

    query = _prep_query(BUFFER_SIZE);
    sprintf(query, SELECT_FIND_SQL_FORMAT, TABLE_NAME, col_name, pattern);

    return query;
}

int run_query(sqlite3* db, const char* query)
{
    sqlite3_stmt*   statement;
    int             status;

    if ((status = sqlite3_prepare_v3(db, query, -1, 0, &statement, NULL)))
    {   
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        return status;
    }
    
    status = sqlite3_step(statement);
    sqlite3_finalize(statement);

    return status;
}

Deck* select(sqlite3* db, const char* select_query)
{
    sqlite3_stmt*   statement;
    int             status;
    Deck*           books;
    Book*           book;

    if ((status = sqlite3_prepare_v3(db, select_query, -1, 0, &statement, NULL)))
        return NULL;
    
    books = DeckCreatePtr(NULL, BookDestroyWRAP);
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        book = BookCreate((char *)sqlite3_column_text(statement, 0),
                            sqlite3_column_int(statement, 1),
                            sqlite3_column_int(statement, 2));
        
        DeckPushBack(books, &book);
    }

    sqlite3_finalize(statement);

    return books;
}

int InsertBook(sqlite3* db, const Book* book)
{
    char* query;
    
    query = get_insert_query(book);
    return run_query(db, query) == SQLITE_DONE ? SQLITE_OK : SQLITE_ERROR;
}

int FindBook(sqlite3* db, const Book* book)
{
    char* query;

    query = get_find_query(book);

    if (run_query(db, query) == SQLITE_ROW)
        return true;
    
    return false;
}

int UpdateBook(sqlite3* db, const Book* book)
{
    char* query;

    query = get_update_query(book);

    return run_query(db, query) == SQLITE_DONE ? SQLITE_OK : SQLITE_ERROR;
}

Deck* SelectAllBooks(sqlite3* db)
{
    char*   query;

    query = get_select_all_query();
    
    return select(db, query);
}

Deck* SelectTopRated(sqlite3* db, int count)
{
    char* query;

    query = get_select_ordered_query(COL_NAMES[SCORE_COL], count);

    return select(db, query);
}

Deck* SelectUnfinished(sqlite3* db, int count)
{
    char* query;

    query = get_select_ordered_with_condition_query(COL_NAMES[STATUS_COL], 2, COL_NAMES[SCORE_COL], count);

    return select(db, query);
}

Deck* SelectUnread(sqlite3* db, int count)
{
    char* query;

    query = get_select_ordered_with_condition_query(COL_NAMES[STATUS_COL], 1, COL_NAMES[SCORE_COL], count);

    return select(db, query);
}

Deck* SelectFind(sqlite3* db, const char* pattern)
{
    char* query;

    query = get_select_find_query(COL_NAMES[NAME_COL], pattern);

    return select(db, query);
}