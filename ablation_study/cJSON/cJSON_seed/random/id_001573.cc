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
//<ID> 1573
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
    cJSON *num_in_array = (cJSON *)0;
    cJSON *num_in_object = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *dup_first = (cJSON *)0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_num_to_array = 0;
    cJSON_bool added_dup_to_root = 0;
    cJSON_bool first_is_number = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_in_array = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    num_in_object = cJSON_AddNumberToObject(root, "direct_value", 42.0);
    added_num_to_array = cJSON_AddItemToArray(arr, num_in_array);
    added_arr_to_root = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_is_number = cJSON_IsNumber(first_elem);
    dup_first = cJSON_Duplicate(first_elem, 1);
    added_dup_to_root = cJSON_AddItemToObject(root, "first_dup", dup_first);

    // step 5: Validate
    validation_score = (int)(num_in_object != (cJSON *)0)
        + (int)added_num_to_array
        + (int)added_arr_to_root
        + (int)(array_size == 1)
        + (int)first_is_number
        + (int)(dup_first != (cJSON *)0)
        + (int)added_dup_to_root;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_arr;
    (void)first_elem;
    (void)num_in_array;
    (void)num_in_object;
    (void)arr;
    (void)added_arr_to_root;
    (void)added_num_to_array;
    (void)added_dup_to_root;
    (void)first_is_number;
    (void)array_size;
    // API sequence test completed successfully
    return 66;
}