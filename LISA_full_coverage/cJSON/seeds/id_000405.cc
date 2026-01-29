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
//<ID> 405
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "items");
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(array, num1);
    cJSON *num2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(array, num2);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    const cJSON *second_item = cJSON_GetArrayItem(array, 1);
    cJSON_bool was_invalid = cJSON_IsInvalid(second_item);
    (void)was_invalid;
    cJSON *new_array = cJSON_CreateArray();
    cJSON_AddItemToArray(new_array, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(new_array, cJSON_CreateNumber(42.0));
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "items", new_array);
    (void)replaced;

    // step 3: Operate
    cJSON_DeleteItemFromArray(new_array, 0);
    int remaining_count = cJSON_GetArraySize(new_array);
    const cJSON *remaining_item = cJSON_GetArrayItem(new_array, 0);
    double numeric_value = cJSON_GetNumberValue(remaining_item);
    cJSON_AddNumberToObject(root, "final_value", numeric_value);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}