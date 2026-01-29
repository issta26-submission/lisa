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
//<ID> 1572
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
    cJSON *added_count = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON_bool added_num1_to_arr = 0;
    cJSON_bool added_num2_to_arr = 0;
    cJSON_bool added_arr_to_obj = 0;
    int array_size = 0;
    int dup_size = 0;
    cJSON_bool first_is_number = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(20.25);

    // step 3: Configure
    added_num1_to_arr = cJSON_AddItemToArray(arr, num1);
    added_num2_to_arr = cJSON_AddItemToArray(arr, num2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);
    added_count = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_is_number = cJSON_IsNumber(first_elem);
    dup_arr = cJSON_Duplicate(retrieved_arr, 1);
    dup_size = cJSON_GetArraySize(dup_arr);

    // step 5: Validate
    validation_score = (int)added_num1_to_arr
        + (int)added_num2_to_arr
        + (int)added_arr_to_obj
        + (int)(added_count != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_size == 2)
        + (int)first_is_number
        + (int)(dup_arr != (cJSON *)0)
        + (int)(dup_size == 2);

    // step 6: Cleanup
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_elem;
    (void)num1;
    (void)num2;
    (void)retrieved_arr;
    (void)added_count;
    // API sequence test completed successfully
    return 66;
}