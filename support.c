#include "books.h"

#include <stdio.h>
#include <stdlib.h>


static void _terminate(sqlite3* db)
{
    WhyEnd();
    sqlite3_close(db);
}

int QuitOnSuccess(sqlite3* db)
{
    _terminate(db);
    printf("%s\n", SUCCESS_MSG);

    return EXIT_SUCCESS;
}

int QuitOnError(sqlite3* db)
{
    _terminate(db);
    fprintf(stderr, "%s\n", ERROR_MSG);

    return EXIT_FAILURE;
}
