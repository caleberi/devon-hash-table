#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "prime.h"



/**
 * @brief resizes the hash table and rehashes all the items
 * @param ht_table* represents the current hash table
 * @param constint represents the new size of the hash table
 * */
static void ht_resize(ht_table *table, const int base_size)
{
    if (base_size < HT_INITIAL_SIZE)
        return;
    ht_table *new_table = ht_create_new_sized_table(base_size);
    for (int i = 0; i < table->size; i++)
    {
        ht_item *item = table->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM)
            ht_insert(new_table, item->key, item->value);
    }
    table->size = new_table->base_size;
    table->count = new_table->count;
    const int temp_size = table->size;
    table->size = new_table->size;
    new_table->size = temp_size;
    ht_item **temp_items = table->items;
    table->items = new_table->items;
    new_table->items = temp_items;
    delete_ht_table(new_table);
}

/**
 * @brief creates a new hash table with a given size by mulitplying the size by 2
 * @param ht_table* represents the current hash table
 * */
static void ht_resize_up(ht_table *ht)
{
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}

/**
 * @brief creates a new hash table with a given size by divide by 2
 * @param ht_table* represents the current hash table
 * */
static void ht_resize_down(ht_table *ht)
{
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}

/**
 * @brief Initializes the ht_items by allocating chunkd of memory for
 * the exact sizeof<struct ht_item> while also taging the function as private
 * @private
 * @param constchar*  k represent the key for hashing and storing in the table
 * @param constchar*  v represent the value to be store after hashing the key <k>
 * */
static ht_item *create_new_ht_item(const char *k, const char *v)
{
    ht_item *item = malloc(sizeof(ht_item));
    item->key = strdup(k);
    item->value = strdup(v);
    return item;
}
/**
 * @brief deletes an existing hashtable node item from the heap
 * to prevent memory leaks
 * @param ht_item* represents the current item to be freed
 * */
static void delete_ht_item(ht_item *item)
{
    free(item->key);
    free(item->value);
    free(item);
}

/**
 * @brief deletes an existing hashtable  from the heap
 * to prevent memory leaks
 * @param ht_table* represents the current table to be freed
 * */
void delete_ht_table(ht_table *table)
{
    for (int i = 0; i < table->size; i++)
    {
        ht_item *item = table->items[i];
        if (item != NULL)
            delete_ht_item(item);
    }
    free(table->items);
    free(table);
}

/**
 * @brief creates a sized hash table
 * @param int size represents the size of the hash table
 * @returns ht_table* represents the hash table
 * */
static ht_table *ht_create_new_sized_table(const int base_size)
{
    ht_table *table = malloc(sizeof(ht_table));
    if (table == NULL)
        exit(EXIT_FAILURE);
    table->base_size = base_size;
    table->size = next_prime(table->size);
    table->count = 0;
    table->items = calloc((size_t)table->size, sizeof(ht_item *));
    return table;
}

/**
 * @brief Creates a new hash tablr
 * @param int default initialization size
 * */
ht_table *ht_new()
{
    return ht_create_new_sized_table(HT_INITIAL_SIZE);
}

/**
 * @brief F[X] Hash function used to create a hash value for the key
 * @param constchar* represents the key to be hashed
 * @param constint prime number to be used for creating the hash value
 * @param constint bucket size of hash table
 * */

static int ht_hash(const char *key, const int prime, const int bucket_size)
{
    long hash = 0;
    const int len_s = strlen(key);
    for (int i = 0; i < len_s; i++)
    {
        hash += (long)pow(prime, len_s - (i + 1)) * key[i];
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
static int ht_get_dhashidx(const char *key, const int bucket_size, const int attempt)
{
    const int hash_a = ht_hash(key, HT_PRIME_X, bucket_size);
    const int hash_b = ht_hash(key, HT_PRIME_Y, bucket_size);
    return (hash_a + (attempt * (hash_b + 1)) % bucket_size);
}

/**
 * @brief inserts a new item into the hash table
 * @param ht_table* represents the current hash table
 * @param constchar* -key represents the key to be hashed
 * @param constchar* -value represents the value to be stored
 * */
ht_table *ht_insert(ht_table *table, const char *key, const char *value)
{
    const int load = table->count * 100 / table->size;
    if (load > 70)
        ht_resize_up(table);

    int idx = ht_get_dhashidx(key, table->size, 0);
    ht_item *item = create_new_ht_item(key, value);
    ht_item *old_item = table->items[idx];
    int i = 1;
    while (old_item != NULL && old_item != &HT_DELETED_ITEM)
    {
        idx = ht_get_dhashidx(key, table->size, i);
        old_item = table->items[idx];
        i++;
    }
    table->items[idx] = item;
    table->count++;
}

/**
 * @brief Finds an item in the hash table
 * @param ht_table* represents the current hash table
 * @param char* -key represents the key to be hashed
 * @return constchar* -value represents the string value found in the hash table
 * */
char *ht_find(ht_table *table, const char *key)
{
    int idx = ht_get_dhashidx(key, table->size, 0);
    ht_item *item = table->items[idx];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
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
 * @param ht_table* represents the current hash table
 * @param constchar* -key represents the key to be hashed
 * */
void ht_delete(ht_table *table, const char *key)
{
    const int load = table->count * 100 / table->size;
    if (load < 10)
        ht_resize_down(table);
    int idx = ht_get_dhashidx(key, table->size, 0);
    ht_item *item = table->items[idx];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(item->key, key) == 0)
            {
                delete_ht_item(item);
                table->items[idx] = &HT_DELETED_ITEM;
            }
        }
        idx = ht_get_dhashidx(key, table->size, i);
        item = table->items[idx];
        i++;
    }
    table->count--;
}
