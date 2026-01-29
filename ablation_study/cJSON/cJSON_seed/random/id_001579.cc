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
//<ID> 1579
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
    cJSON *dup_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *direct_num = (cJSON *)0;
    cJSON *added_direct_ptr = (cJSON *)0;
    cJSON_bool added_num1_to_array = 0;
    cJSON_bool added_dup_to_array = 0;
    cJSON_bool added_array_to_root = 0;
    int array_size = 0;
    cJSON_bool first_is_number = 0;
    cJSON_bool direct_is_number = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(-3.25);

    // step 3: Configure
    added_num1_to_array = cJSON_AddItemToArray(arr, num1);
    dup_num = cJSON_Duplicate(num2, 0);
    added_dup_to_array = cJSON_AddItemToArray(arr, dup_num);
    added_array_to_root = cJSON_AddItemToObject(root, "values", arr);
    added_direct_ptr = cJSON_AddNumberToObject(root, "direct", 7.75);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_is_number = cJSON_IsNumber(first_elem);
    direct_num = cJSON_GetObjectItem(root, "direct");
    direct_is_number = cJSON_IsNumber(direct_num);

    // step 5: Validate
    validation_score = (int)added_num1_to_array
        + (int)added_dup_to_array
        + (int)added_array_to_root
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_size == 2)
        + (int)(first_elem != (cJSON *)0)
        + (int)first_is_number
        + (int)(added_direct_ptr != (cJSON *)0)
        + (int)direct_is_number;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)num2;
    (void)first_elem;
    (void)direct_num;
    (void)added_direct_ptr;
    // API sequence test completed successfully
    return 66;
}