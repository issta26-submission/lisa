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
//<ID> 1578
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
    cJSON *added_number_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *duplicated = (cJSON *)0;
    int array_size = 0;
    cJSON_bool first_is_number = 0;
    int validation_score = 0;
    cJSON_bool added_arr_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    added_arr_flag = cJSON_AddItemToObject(root, "values", arr);
    added_number_item = cJSON_AddNumberToObject(root, "sum", 4.0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_is_number = cJSON_IsNumber(first_elem);
    duplicated = cJSON_Duplicate(first_elem, 1);

    // step 5: Validate
    validation_score = (int)added_arr_flag
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_size == 2)
        + (int)first_is_number
        + (int)(duplicated != (cJSON *)0)
        + (int)(added_number_item != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(duplicated);
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_elem;
    (void)array_size;
    (void)first_is_number;
    (void)num1;
    (void)num2;
    (void)added_number_item;
    // API sequence test completed successfully
    return 66;
}