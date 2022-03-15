#include "books.h"

#include <stdlib.h>
#include <string.h>

char* input_strings[] = {"unknown","not read,unread", "not finished,reading", "finished,read", 0};

int BookInit(Book* book, char* name, int score, STATUS status)
{
    book->name = name;
    book->score = score;
    book->status = status;

    return WHY_OK;
}

Book* BookCreate(const char* name, int score, STATUS status)
{
    Book* book;
    char* _name;

    if (!(book = malloc(sizeof(*book))))
        return NULL;
    
    if (!(_name = strdup(name)))
    {
        free(book);
        return NULL;
    }

    BookInit(book, _name, score, status);

    return book;
}

void BookDestroy(Book* book)
{
    if (book)
    {
        free(book->name);
        free(book);
    }
}

void BookDestroyWRAP(void* book)
{
    BookDestroy(*(Book **)book);
}

static int score_from_string(const char* string)
{
    Int number;

    if (ParseInt(&number, string) <= 0)
        return WHY_ERROR;
    
    if (number <= 0)
        number = SCORE_DEFAULT;
    else if (number > SCORE_MAX)
        number = SCORE_MAX;
    
    return number;
}

static int status_from_string(const char* string)
{
    char*   lower;
    Deck*   strings;
    Uint    n;

    lower = StringToLower(string);
    n = 0;
    while (input_strings[n])
    {
        strings = StringSplit(input_strings[n], ',');
        if (DeckBinSearch(strings, &lower, CompareCstr))
        {
            free(lower);
            DeckDestroy(strings);
            return n;
        }
        
        DeckDestroy(strings);
        ++ n;
    }

    free(lower);

    return STATUS_UNKONOWN;
}

char* cut_extension(char* name)
{
    Int index;

    index = StringFindC(name, '.');

    if (index != NOT_FOUND)
        name[index] = '\0';
    
    return name;
}

char* drop_folder(char* name)
{
    Int index;

    index = StringFindCLast(name, '/');
    if (index != NOT_FOUND)
        return name + index + 1;
    
    return name;
}

char* get_book_name(const char* argument)
{
    char* name;

    name = strdup(argument);
    WhySavePtr(&name);

    name = cut_extension(name);
    name = drop_folder(name);

    return name;
}

int BookInitFromStrings(Book* book, int count, char** strings)
{
    int     score;
    int     status;
    char*   name;

    if (count == 1)
        return BookInit(book, strings[0], SCORE_DEFAULT, STATUS_DEFAULT);
    
    if (count != 3)
        return WHY_ERROR;

    score = score_from_string(strings[1]);
    status = status_from_string(strings[2]);

    if (score == WHY_ERROR || status == STATUS_UNKONOWN)
        return WHY_ERROR;
    
    name = get_book_name(strings[0]);

    return BookInit(book, name, score, status);
}