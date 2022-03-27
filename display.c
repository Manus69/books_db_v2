#include "books.h"

#include <stdio.h>

#define NAME_FIELD_WIDTH        (30)
#define NAME_FIELD_NCHARS       (30)
#define SCORE_FIELD_WIDTH       (5)
#define STATUS_FIELD_WIDTH      (5)
#define NAME_FORMAT             ("%-*.*s")
#define SCORE_FORMAT            ("| %*d |")
#define SCORE_HEADER_FORMAT     ("| %s |")
#define STATUS_HEADER_FORMAT    (" %s")
#define STATUS_FORMAT           (" %s")
#define SEPARATOR               ("---------------------------------------------")

void PrintHeader()
{
    printf(NAME_FORMAT, NAME_FIELD_WIDTH, NAME_FIELD_NCHARS, COL_NAMES[NAME_COL]);
    printf(SCORE_HEADER_FORMAT, COL_NAMES[SCORE_COL]);
    printf(STATUS_HEADER_FORMAT, COL_NAMES[STATUS_COL]);
    printf("\n%s\n", SEPARATOR);
}

void PrintBook(const Book* book)
{
    printf(NAME_FORMAT, NAME_FIELD_WIDTH, NAME_FIELD_NCHARS, book->name);
    printf(SCORE_FORMAT, SCORE_FIELD_WIDTH, book->score);
    printf(STATUS_FORMAT, STATUS_STRINGS[book->status]);
    printf("\n");
}

void PrintBookWRAP(const void* book)
{
    PrintBook(*(Book **)book);
}

void PrintResultTable(const Deck* books)
{
    if (DeckNItems(books))
    {
        PrintHeader();
        PrintDeck(books, PrintBookWRAP);
    }
}