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
//<ID> 627
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "scale", 2.5);
    cJSON_AddNullToObject(root, "missing");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(1.25);
    cJSON *n1 = cJSON_CreateNumber(2.75);
    cJSON *n2 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(values, 0);
    cJSON *a1 = cJSON_GetArrayItem(values, 1);
    cJSON *a2 = cJSON_GetArrayItem(values, 2);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double sum = v0 + v1 + v2;
    double scale = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "scale"));
    double scaled_sum = sum * scale;
    cJSON_bool enabled_is_bool = cJSON_IsBool(cJSON_GetObjectItem(root, "enabled"));
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "scaled_sum", scaled_sum);
    cJSON_AddBoolToObject(root, "enabled_is_bool", enabled_is_bool);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}