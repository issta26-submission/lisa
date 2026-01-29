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
//<ID> 607
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n0 = cJSON_CreateNumber(2.0);
    cJSON *n1 = cJSON_CreateNumber(4.0);
    cJSON *n2 = cJSON_CreateNumber(6.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "threshold", 5.0);
    cJSON *raw_blob = cJSON_AddRawToObject(root, "raw_blob", "{\"enabled\":true,\"ratio\":0.75}");

    // step 2: Configure
    int count = cJSON_GetArraySize(items);
    cJSON *a0 = cJSON_GetArrayItem(items, 0);
    cJSON *a1 = cJSON_GetArrayItem(items, 1);
    cJSON *a2 = cJSON_GetArrayItem(items, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON_AddNumberToObject(root, "average", average);
    cJSON_bool config_is_object = cJSON_IsObject(config);
    (void)raw_blob; // keep raw_blob in data flow (used via creation)

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement, "avg_times_two", average * 2.0);
    cJSON_AddStringToObject(replacement, "status", "replaced");
    cJSON_bool replaced_flag = cJSON_ReplaceItemInObject(root, "config", replacement);
    (void)config_is_object;
    (void)replaced_flag;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}