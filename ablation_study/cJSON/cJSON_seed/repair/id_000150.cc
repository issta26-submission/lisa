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
//<ID> 150
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
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *n0 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(items, n0);
    cJSON *n1 = cJSON_CreateNumber(7.25);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(-1.75);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    cJSON *scale = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "scale", scale);
    cJSON *threshold = cJSON_CreateNumber(5.0);
    cJSON_AddItemToObject(root, "threshold", threshold);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it1 = cJSON_GetArrayItem(items, 1);
    cJSON *it2 = cJSON_GetArrayItem(items, 2);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double v2 = cJSON_GetNumberValue(it2);
    double sum = v0 + v1 + v2;
    double scaled = sum * cJSON_GetNumberValue(scale);
    cJSON *scaled_item = cJSON_CreateNumber(scaled);
    cJSON_AddItemToObject(root, "scaled_sum", scaled_item);
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}