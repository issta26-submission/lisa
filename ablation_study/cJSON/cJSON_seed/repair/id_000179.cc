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
//<ID> 179
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
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    double previous_n1 = cJSON_SetNumberHelper(n1, 15.0);
    cJSON_AddFalseToObject(metrics, "valid");
    cJSON *stats = cJSON_AddObjectToObject(metrics, "stats");
    cJSON *count = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(stats, "count", count);

    // step 3: Operate and Validate
    cJSON *av0 = cJSON_GetArrayItem(values, 0);
    cJSON *av1 = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(av0);
    double v1 = cJSON_GetNumberValue(av1);
    double sum = v0 + v1 + previous_n1 - previous_n1; // meaningful flow while keeping straight-line code
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(metrics, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}