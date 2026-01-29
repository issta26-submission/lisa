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
//<ID> 1574
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
    cJSON *dup_num1 = (cJSON *)0;
    cJSON *direct_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *retrieved_direct = (cJSON *)0;
    cJSON_bool added_num1_to_arr = 0;
    cJSON_bool added_dup_to_arr = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_num2_to_root = 0;
    cJSON_bool is_first_number = 0;
    cJSON_bool is_direct_number = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);

    // step 3: Configure
    added_num1_to_arr = cJSON_AddItemToArray(arr, num1);
    dup_num1 = cJSON_Duplicate(num1, 0);
    added_dup_to_arr = cJSON_AddItemToArray(arr, dup_num1);
    added_arr_to_root = cJSON_AddItemToObject(root, "values", arr);
    added_num2_to_root = cJSON_AddItemToObject(root, "single", num2);
    direct_num = cJSON_AddNumberToObject(root, "direct_number", 3.1415);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    is_first_number = cJSON_IsNumber(first_elem);
    retrieved_direct = cJSON_GetObjectItem(root, "direct_number");
    is_direct_number = cJSON_IsNumber(retrieved_direct);

    // step 5: Validate
    validation_score = (int)added_num1_to_arr
        + (int)added_dup_to_arr
        + (int)added_arr_to_root
        + (int)added_num2_to_root
        + (int)(direct_num != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(arr_size == 2)
        + (int)(first_elem != (cJSON *)0)
        + (int)is_first_number
        + (int)is_direct_number;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_direct;
    (void)direct_num;
    (void)first_elem;
    (void)retrieved_arr;
    (void)dup_num1;
    (void)num1;
    (void)num2;
    // API sequence test completed successfully
    return 66;
}