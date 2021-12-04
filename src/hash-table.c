#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../lib/hash-table.h"
#include "../lib/prime.h"



/**
 * @brief resizes the hash table and rehashes all the items
 * .Note : it should not be < 50
 * @param Table* represents the current hash table
 * @param constint represents the new size of the hash table
 * */
static Table* ht_resize(Table *table, const int base_size)
{
    if (base_size < HT_INITIAL_SIZE)
        return table;
    Table *new_table = ht_create_new_sized_table(base_size); // brand new table
    for (int i = 0; i < table->size; i++)
    {
        Item *item = table->items[i];
        if (item != NULL && !ht_cell_empty(item))
            ht_insert(new_table, item->key, item->value);
    }
    delete_Table(table);
    return new_table;
}

/**
 * @brief checks to see if an item property are 
 * empty
 * @param item 
 * @return true 
 * @return false 
 */
static bool ht_cell_empty(Item* item){
    return item->key == HT_EMPTY_ITEM.key && item->value == HT_EMPTY_ITEM.value;
}

/**
 * @brief creates a new hash table with a given size by mulitplying the size by 2
 * @param Table* represents the new hash table
 * */
static inline Table* ht_resize_up(Table *ht)
{
    const int new_size = ht->base_size * 2;
    return ht_resize(ht, new_size);
}

/**
 * @brief creates a new hash table with a given size by divide by 2
 * @param Table* represents the new hash table
 * */
static inline Table* ht_resize_down(Table *ht)
{
    const int new_size = ht->base_size / 2;
    return ht_resize(ht, new_size);
}


/**
 * @brief Initializes the ht_items by allocating chunk of memory for
 * the exact `sizeof<struct Item>` while also taging the function as private
 * @private
 * @param constchar*  k represent the key for hashing and storing in the table
 * @param constchar*  v represent the value to be store after hashing the key <k>
 * */
static inline Item *create_new_item(const char *k, const char *v)
{
    Item *item = malloc(sizeof(Item));
    item->key = strdup(k);
    item->value = strdup(v);
    return item;
}




/**
 * @brief deletes an existing hashtable node item from the heap
 * to prevent memory leaks
 * @param Item* represents the current item to be freed
 * */
static inline void delete_ht_item(Item *item)
{
    free(item->key);
    free(item->value);
    free(item);
}

/**
 * @brief deletes an existing hashtable  from the heap
 * to prevent memory leaks
 * @param Table* represents the current table to be freed
 * */
void delete_Table(Table *table)
{
    for (int i = 0; i < table->size; i++)
    {
        Item *item = table->items[i];
        if (item != NULL)
            delete_ht_item(item);
    }
    free(table->items);
    free(table);
}

/**
 * @brief creates a sized hash table
 * @param int size represents the size of the hash table
 * @returns Table* represents the hash table
 * */
static inline Table *ht_create_new_sized_table(const int base_size)
{
    Table *table = malloc(sizeof(Table));
    if (table == NULL)
        exit(EXIT_FAILURE);
    table->base_size = base_size;
    table->size = next_prime(base_size);
    table->count = 0;
    table->items = calloc((size_t)table->size, sizeof(Item *));
    return table;
}

/**
 * @brief Creates a new hash tablr
 * @param int default initialization size
 * */
Table *ht_new()
{
    return ht_create_new_sized_table(HT_INITIAL_SIZE);
}

/**
 * @brief F[X] Hash function used to create a hash value for the key
 * @param constchar* represents the key to be hashed
 * @param constint prime number to be used for creating the hash value
 * @param constint bucket size of hash table
 * */

static inline int ht_hash(const char *key, const int prime, const int bucket_size)
{
    long hash = 0;
    const int len_s = strlen(key);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(prime, len_s - (i+1)) * key[i];
        hash = hash % bucket_size;
    }
    return (int)hash;
}

/**
 * @brief retreives the index for a bucket after double hashing 
 * @param constchar* represents the key to be hashed
 * @param constint number of buckets in the hash table
 * @param constint attempts to find a free bucket
 **/
static inline int ht_get_dhashidx(const char *key, const int bucket_size, const int attempt)
{
    int hash_a = ht_hash(key, HT_PRIME_X, bucket_size);
    int hash_b = ht_hash(key, HT_PRIME_Y, bucket_size);
    if (hash_b % bucket_size == 0) {
        hash_b = 1;
    }
    return (hash_a + (attempt * (hash_b)) % bucket_size);
}

/**
 * @brief inserts a new item into the hash table
 * @param Table* represents the current hash table
 * @param constchar* -key represents the key to be hashed
 * @param constchar* -value represents the value to be stored
 * */
Table *ht_insert(Table *table, const char *key, const char *value)
{
    const int load = table->count * 100 / table->size;
    if (load > 70)
        table = ht_resize_up(table);

    int idx = ht_get_dhashidx(key, table->size, 0);
    Item *item = create_new_item(key, value);
    Item* old_item = table->items[idx];
    int i = 1;
    while (old_item != NULL && !ht_cell_empty(old_item))
    {
        idx = ht_get_dhashidx(key, table->size, i);
        old_item = table->items[idx];
        i++;
    }
    table->items[idx] = item;
    table->count++;
    return table;
}

/**
 * @brief Finds an item in the hash table
 * @param Table* represents the current hash table
 * @param char* -key represents the key to be hashed
 * @return constchar* -value represents the string value found in the hash table
 * */
char *ht_find(Table *table, const char *key)
{
    int idx = ht_get_dhashidx(key, table->size, 0);
    Item *item = table->items[idx];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_EMPTY_ITEM)
        {
            if (strcmp(item->key, key) == 0)
                return item->value;
        }
        idx = ht_get_dhashidx(key, table->size, i);
        item = table->items[idx];
        i++;
    }
    return NULL;
}

/**
 * @brief deletes an item from the hash table
 * @param Table* represents the current hash table
 * @param constchar* -key represents the key to be hashed
 * */
void ht_delete(Table *table, const char *key)
{
    const int load = table->count * 100 / table->size;
    if (load < 10)
        ht_resize_down(table);
    int idx = ht_get_dhashidx(key, table->size, 0);
    Item *item = table->items[idx];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_EMPTY_ITEM)
        {
            if (strcmp(item->key, key) == 0)
            {
                delete_ht_item(item);
                table->items[idx] = &HT_EMPTY_ITEM;
            }
        }
        idx = ht_get_dhashidx(key, table->size, i);
        item = table->items[idx];
        i++;
    }
    table->count--;
}
