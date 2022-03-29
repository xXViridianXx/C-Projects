#pragma once

typedef struct PQEntry PQEntry;

PQEntry *entry_create(char *author, double distance);

void entry_delete(PQEntry **e);

void entry_print(PQEntry *e);
