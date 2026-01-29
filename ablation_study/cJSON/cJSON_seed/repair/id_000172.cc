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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON *flags = cJSON_AddObjectToObject(metrics, "flags");
    cJSON *enabled = cJSON_AddFalseToObject(flags, "enabled");

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "values", values);
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, n1);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    cJSON_SetNumberHelper(cJSON_GetArrayItem(values, 1), v0 + 3.0);
    double sum = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0)) + cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}