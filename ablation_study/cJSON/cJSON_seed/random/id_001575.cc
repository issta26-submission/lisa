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
//<ID> 1575
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
    cJSON *num_via_add = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *dup_first = (cJSON *)0;
    cJSON_bool added_num1_to_arr = 0;
    cJSON_bool added_num2_to_arr = 0;
    cJSON_bool added_arr_to_root = 0;
    int array_size = 0;
    int dup_array_size = 0;
    cJSON_bool first_is_number = 0;
    cJSON_bool dup_first_is_number = 0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_via_add = cJSON_AddNumberToObject(root, "pi", 3.1415);
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    added_num1_to_arr = cJSON_AddItemToArray(arr, num1);
    added_num2_to_arr = cJSON_AddItemToArray(arr, num2);
    added_arr_to_root = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    first_elem = cJSON_GetArrayItem(arr, 0);
    array_size = cJSON_GetArraySize(arr);
    first_is_number = cJSON_IsNumber(first_elem);
    dup_root = cJSON_Duplicate(root, 1);
    dup_arr = cJSON_GetObjectItem(dup_root, "values");
    dup_array_size = cJSON_GetArraySize(dup_arr);
    dup_first = cJSON_GetArrayItem(dup_arr, 0);
    dup_first_is_number = cJSON_IsNumber(dup_first);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)(arr != (cJSON *)0)
        + (int)(num_via_add != (cJSON *)0)
        + (int)added_num1_to_arr
        + (int)added_num2_to_arr
        + (int)added_arr_to_root
        + (int)(first_elem != (cJSON *)0)
        + (int)(array_size == 2)
        + (int)first_is_number
        + (int)(dup_root != (cJSON *)0)
        + (int)(dup_arr != (cJSON *)0)
        + (int)(dup_array_size == 2)
        + (int)(dup_first != (cJSON *)0)
        + (int)dup_first_is_number;

    // step 6: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    (void)validation_score;
    (void)num_via_add;
    (void)num1;
    (void)num2;
    (void)first_elem;
    (void)dup_arr;
    (void)dup_first;
    // API sequence test completed successfully
    return 66;
}