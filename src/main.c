#include <stdio.h>
#include "../lib/hash-table.h"

int main()
{
    Table* ht = ht_new();
    ht_insert(ht,"34","Mike");
    ht_insert(ht,"56","Time");
    ht_insert(ht,"104","Love");
    const char* value = ht_find(ht,"34");
    printf("%s",value);
    delete_Table(ht);

}