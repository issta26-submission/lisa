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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *bf = cJSON_CreateFalse();
    cJSON *parsed = cJSON_Parse("{\"nested\": 123.5, \"flag\": false}");

    // step 2: Configure
    cJSON_AddItemToArray(arr, bf);
    cJSON_AddItemToArray(arr, parsed);
    cJSON *extra_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, extra_num);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first);
    cJSON *parsed_nested = cJSON_GetObjectItem(parsed, "nested");
    double nested_val = cJSON_GetNumberValue(parsed_nested);
    cJSON_SetNumberHelper(parsed_nested, nested_val + 1.5);
    double new_nested_val = cJSON_GetNumberValue(parsed_nested);
    cJSON *copied = cJSON_Duplicate(parsed, 1);
    cJSON_bool are_equal = cJSON_Compare(parsed, copied, 1);
    cJSON_Delete(copied);
    int validation_sum = (int)first_is_false + size + (int)(new_nested_val > nested_val) + (int)are_equal;
    cJSON_SetNumberHelper(extra_num, (double)validation_sum);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}