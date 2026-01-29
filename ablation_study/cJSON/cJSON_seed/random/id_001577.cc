#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1577
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *standalone = (cJSON *)0;
    cJSON *duplicated = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool added_num1_to_arr = 0;
    cJSON_bool added_arr_to_root = 0;
    int arr_size = 0;
    int is_num = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);

    // step 3: Configure
    added_num1_to_arr = cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    added_arr_to_root = cJSON_AddItemToObject(root, "numbers", arr);
    standalone = cJSON_AddNumberToObject(root, "standalone", 3.14159);

    // step 4: Operate
    duplicated = cJSON_Duplicate(root, 1);
    retrieved_arr = cJSON_GetObjectItem(duplicated, "numbers");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_num = cJSON_GetArrayItem(retrieved_arr, 0);
    is_num = (int)cJSON_IsNumber(retrieved_num);

    // step 5: Validate
    validation_score = (int)added_num1_to_arr
        + (int)added_arr_to_root
        + (int)(standalone != (cJSON *)0)
        + (int)(duplicated != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(arr_size == 2)
        + is_num;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(duplicated);
    (void)validation_score;
    (void)arr_size;
    (void)retrieved_num;
    (void)retrieved_arr;
    (void)num1;
    (void)num2;
    (void)standalone;
    (void)added_num1_to_arr;
    (void)added_arr_to_root;
    // API sequence test completed successfully
    return 66;
}