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
//<ID> 426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[4] = {10, 20, 30, 40};
    cJSON *ints = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *orig_value = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "value", orig_value);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(ints, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *replacement = cJSON_CreateNumber(first_val + 0.5);
    cJSON_ReplaceItemInObject(root, "value", replacement);

    // step 3: Operate
    cJSON *second = cJSON_GetArrayItem(ints, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *third = cJSON_GetArrayItem(ints, 2);
    double third_val = cJSON_GetNumberValue(third);
    double sum_val = first_val + second_val + third_val;
    cJSON *sum_item = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate and Cleanup
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    cJSON *enabled_copy = cJSON_CreateBool(has_enabled);
    cJSON_AddItemToObject(root, "enabled_copy", enabled_copy);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}