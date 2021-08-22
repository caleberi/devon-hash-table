#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/**
 * @brief  Hash table class.
 * @author Hebronace
 * @date   2019-04-05
 * @details Implementation of a string based hash table with chaining.
 *  */
#include <stdlib.h>
#define HT_PRIME_X 2
#define HT_PRIME_Y 3
#define HT_INITIAL_SIZE 50

/**
 * @brief Structural definition for a hash table node
 * */
struct hash_table_node
{
    char *key;
    char *value;
};

typedef struct hash_table_node ht_item;

/**
 * @brief Structural definition for a hash table  with array of pointers 
 * to  struct hash_table_node
 * */
struct hash_table
{
    int size;
    int base_size;
    int count;
    ht_item **items;
};
typedef struct hash_table ht_table;

static ht_item HT_DELETED_ITEM = {NULL, NULL};
static void ht_resize(ht_table *, const int);
static void ht_resize_up(ht_table *);
static void ht_resize_down(ht_table *);
static ht_item *create_new_ht_item(const char *, const char *);
static void delete_ht_item(ht_item *);
static ht_table *ht_create_new_sized_table(const int);
static int ht_hash(const char *, const int, const int);
static int ht_get_dhashidx(const char *, const int, const int);
ht_table *ht_insert(ht_table *, const char *, const char *);
char *ht_find(ht_table *, const char *);
void ht_delete(ht_table *, const char *);
ht_table *ht_new();
void delete_ht_table(ht_table *);

#endif // HASH_TABLE_H