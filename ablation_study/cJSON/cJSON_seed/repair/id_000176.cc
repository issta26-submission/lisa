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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *stats = cJSON_AddObjectToObject(meta, "stats");
    cJSON *enabled_false = cJSON_AddFalseToObject(meta, "enabled");

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(meta, "values", values);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_SetNumberHelper(n0, 15.0);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    double sum = v0 + v1;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *stats_count = cJSON_CreateNumber((double)cJSON_GetArraySize(values));
    cJSON_AddItemToObject(stats, "count", stats_count);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}