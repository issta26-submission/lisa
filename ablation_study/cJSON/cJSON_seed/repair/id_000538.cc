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
//<ID> 538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *nested_val = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(nested, "value", nested_val);

    // step 2: Configure
    cJSON *dup_nested = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "nested_copy", dup_nested);

    // step 3: Operate
    cJSON *got_value = cJSON_GetObjectItem(nested, "value");
    double value = cJSON_GetNumberValue(got_value);
    cJSON *double_item = cJSON_CreateNumber(value * 2.0);
    cJSON_AddItemToObject(root, "double_value", double_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *copy = cJSON_GetObjectItem(root, "nested_copy");
    cJSON_bool equal = cJSON_Compare(nested, copy, 1);
    cJSON *compare_num = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "compare_result", compare_num);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}