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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    int nums[] = {10, 20, 30};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "mixed", arr);
    cJSON_AddItemToObject(root, "ints", int_arr);

    // step 3: Operate & Validate
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON *second_elem = cJSON_GetArrayItem(arr, 1);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON_bool second_is_false = cJSON_IsFalse(second_elem);
    cJSON *first_copy = cJSON_CreateNumber(first_val);
    cJSON *second_bool_copy = cJSON_CreateBool(second_is_false);
    cJSON_AddItemToObject(root, "first_value_copy", first_copy);
    cJSON_AddItemToObject(root, "second_is_false_copy", second_bool_copy);
    cJSON *ints_second = cJSON_GetArrayItem(int_arr, 1);
    double ints_second_val = cJSON_GetNumberValue(ints_second);
    double sum = first_val + ints_second_val;
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum));

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}