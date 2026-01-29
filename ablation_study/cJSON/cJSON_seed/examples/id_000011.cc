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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *initial_null = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, initial_null);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *nil_marker = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nil", nil_marker);
    cJSON *num_item = cJSON_AddNumberToObject(root, "value", 3.1415);
    cJSON *replacement_str = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 0, replacement_str);

    // step 2: Operate
    cJSON *arr0 = cJSON_GetArrayItem(arr, 0);
    cJSON *comparison_str = cJSON_CreateString("replaced");
    cJSON_bool are_equal = cJSON_Compare(arr0, comparison_str, 1);
    double numeric_value = cJSON_GetNumberValue(num_item);
    cJSON_bool nil_check = cJSON_IsNull(cJSON_GetObjectItem(root, "nil"));

    // step 3: Validate
    cJSON_AddBoolToObject(root, "equal", are_equal);
    cJSON_AddBoolToObject(root, "nil_is_null", nil_check);
    cJSON_AddNumberToObject(root, "extracted_value", numeric_value);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}