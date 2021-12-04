#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/**
 * @brief  Hash table class.
 * @author Hebronace
 * @date   2019-04-05
 * @details Implementation of a string based hash table with chaining.
 *  */
#include <stdlib.h>
#include <stdbool.h>

#define HT_PRIME_X 2
#define HT_PRIME_Y 3
#define HT_INITIAL_SIZE 50


/**
 * @brief Structural definition for a hash table node
 * */
struct hash_table_node
{
    char *key;  //for key storage {must be hashable}
    char *value; // for value storage
};

typedef struct hash_table_node Item;

/**
 * @brief Structural definition for a hash table  with array of pointers 
 * to  struct hash_table_node
 * */
struct hash_table
{
    // defined data size of the table
    int size; 
    // the original size before further expansion
    int base_size; 
    // number of item in the table
    int count; 
    // pointers to each  an array of `Item`
    Item **items; 
};

typedef struct hash_table Table;



static Item HT_EMPTY_ITEM = {.key=NULL, .value=NULL};

static inline Table* ht_resize(Table *, const int);

static inline bool ht_cell_empty(Item* item);

static inline Table* ht_resize_up(Table *);

static inline Table* ht_resize_down(Table *);

static inline Item *create_new_item(const char *, const char *);

static inline void delete_item(Item *);

static inline Table *ht_create_new_sized_table(const int);

static inline int ht_hash(const char *, const int, const int);

static  inline int ht_get_dhashidx(const char *, const int, const int);

Table *ht_insert(Table *, const char *, const char *);

char *ht_find(Table *, const char *);

void ht_delete(Table *, const char *);

Table *ht_new();

void delete_Table(Table *);



#endif // HASH_TABLE_H