#include <stdio.h>

#include "../lib/prime.h"

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

static FILE* temp_file = NULL;

int init_suite_1(void){
    temp_file = fopen("temp.txt", "w+");
    if (temp_file==NULL)
        return -1;
    return 0;
}

int cleanup_suite_1(void){
    int code = fclose(temp_file);
    if (code!= 0)
        return -1;
    return 0;
}

void test_is_prime(void){
    CU_ASSERT_EQUAL(is_prime(47),1);
}

void test_is_next_prime(void){
    CU_ASSERT_EQUAL(next_prime(48),53);
} 

int main(void)
{
    CU_pSuite suite = NULL;

    if(CU_initialize_registry()!=CUE_SUCCESS)
        return CU_get_error();
    
    suite =  CU_add_suite("Testing mathematical functions",init_suite_1,cleanup_suite_1);
    if(suite==NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    if(
        CU_add_test(suite,"Test IsPrimeNumber()",test_is_prime)==NULL||
        CU_add_test(suite,"Test IsNextPrimeNumber()",test_is_next_prime)==NULL
    ){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_NORMAL);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}


