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
//<ID> 626
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
    cJSON *n0 = cJSON_CreateNumber(4.5);
    cJSON *n1 = cJSON_CreateNumber(5.5);
    cJSON *n2 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON_AddNullToObject(root, "missing");
    cJSON_AddNumberToObject(root, "threshold", 6.0);
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);

    // step 3: Operate and Validate
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    cJSON *v2 = cJSON_GetArrayItem(values, 2);
    double d0 = cJSON_GetNumberValue(v0);
    double d1 = cJSON_GetNumberValue(v1);
    double d2 = cJSON_GetNumberValue(v2);
    double sum = d0 + d1 + d2;
    double average = sum / 3.0;
    cJSON_AddNumberToObject(root, "average", average);
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "enabled_is_bool", (double)enabled_is_bool);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}