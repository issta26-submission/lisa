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
//<ID> 1587
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
    cJSON *str_item = (cJSON *)0;
    cJSON *str_dup = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *first_str = (char *)0;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_dup_to_root = 0;
    int array_size = 0;
    cJSON_bool first_is_string = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("alpha");

    // step 3: Configure
    added_to_array = cJSON_AddItemToArray(arr, str_item);
    added_arr_to_root = cJSON_AddItemToObjectCS(root, "elements", arr);
    str_dup = cJSON_Duplicate(str_item, 1);
    added_dup_to_root = cJSON_AddItemToObjectCS(root, "duplicate", str_dup);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "elements");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(first_elem);
    first_is_string = cJSON_IsString(first_elem);

    // step 5: Validate
    validation_score = (int)added_to_array
        + (int)added_arr_to_root
        + (int)added_dup_to_root
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_size == 1)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_str != (char *)0)
        + (int)first_is_string;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_str;
    (void)first_elem;
    (void)retrieved_arr;
    (void)arr;
    (void)str_item;
    (void)str_dup;
    (void)added_to_array;
    (void)added_arr_to_root;
    (void)added_dup_to_root;
    (void)array_size;
    (void)first_is_string;
    // API sequence test completed successfully
    return 66;
}