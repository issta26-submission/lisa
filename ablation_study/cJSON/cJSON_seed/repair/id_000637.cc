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
//<ID> 637
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.0));

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(ver));
    double updated_threshold = cJSON_SetNumberHelper(threshold, 20.5);
    cJSON_AddNumberToObject(config, "threshold_after_sethelper", updated_threshold);

    // step 3: Operate and Modify
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_prealloc = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON *new_threshold = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(config, threshold, new_threshold);
    double new_set = cJSON_SetNumberHelper(new_threshold, 84.0);
    cJSON_AddNumberToObject(config, "threshold_final", new_set);
    int arr_count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "values_count", (double)arr_count);
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    double sum01 = cJSON_GetNumberValue(v0) + cJSON_GetNumberValue(v1);
    cJSON_AddNumberToObject(root, "sum_first_two", sum01);
    cJSON_AddNumberToObject(root, "printed_prealloc_flag", (double)printed_prealloc);
    cJSON_AddNumberToObject(root, "replace_result", (double)replaced);

    // step 4: Validate and Cleanup
    cJSON *check_enabled = cJSON_GetObjectItem(config, "enabled");
    cJSON_AddNumberToObject(root, "enabled_is_bool", (double)cJSON_IsBool(check_enabled));
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}