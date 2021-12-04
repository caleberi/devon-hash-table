#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../lib/hash-table.h"

struct Entry{
    char* key;
    char* value;
};

static Table* table;

int initialize_hash_table_suite(void){
    if((table=ht_new())==NULL){
        return 1;
    }
    return 0;
}

int cleanup_hash_table_suite(void){
    if(table!=NULL)
        delete_Table(table);
    if(table==NULL)
        return 1;
    return 0;
}

void test_create_table(){
    CU_ASSERT_PTR_NOT_NULL(table);
}

void test_insert_into_table(){
    struct Entry entries[4] ={
        {.key="1",.value="mattew"},
        {.key="2",.value="john"},
        {.key="3",.value="james"},
        {.key="4",.value="andrew"}
    };
    for(int i=0;i<4;++i){
        struct Entry current = entries[i];
        ht_insert(table,current.key,current.value);
    }
    CU_ASSERT_EQUAL(table->count,4);
}

void test_find_key_in_table(){
    char* keys[4] = {"1","2","3","4"};
    char* values[4] = {"mattew","john","james","andrew"};
    for (int i = 0; i < 4; i++)
    {
        CU_ASSERT_STRING_EQUAL(ht_find(table,keys[i]),values[i]);
    }
}

int main(){
    // creating registry
    if(CU_initialize_registry()==CUE_NOMEMORY){
        printf("ERROR: {%s} \n",CU_get_error_msg());
        CU_cleanup_registry();
        return CU_get_error();
    }
    // creating suite
    CU_pSuite hash_table_suite = CU_add_suite("TestSuite::Hash_Table",initialize_hash_table_suite,cleanup_hash_table_suite);
    if(hash_table_suite==NULL){
        printf("ERROR: {%s} \n",CU_get_error_msg());
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if(
        CU_add_test(hash_table_suite,"Should be able to create an table",test_create_table)==NULL||
        CU_add_test(hash_table_suite,"Should be able to insert data into table",test_insert_into_table)==NULL||
        CU_add_test(hash_table_suite,"Should be able to find data into table",test_find_key_in_table)==NULL
    ){
        printf("ERROR: {%s} \n",CU_get_error_msg());
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
}