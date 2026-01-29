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
//<ID> 1073
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"example\",\"config\":{\"enabled\":false},\"metric\":1.5,\"values\":[0.5,1.5]}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON_AddTrueToObject(config, "toggled");
    double updated_values[] = {2.2, 3.3, 4.4};
    cJSON *new_values_array = cJSON_CreateDoubleArray(updated_values, 3);
    cJSON *new_metric = cJSON_CreateNumber(42.0);

    // step 3: Operate
    cJSON_ReplaceItemInObjectCaseSensitive(root, "values", new_values_array);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "metric", new_metric);
    cJSON *metric_item = cJSON_GetObjectItem(root, "metric");
    double metric_value = cJSON_GetNumberValue(metric_item);
    cJSON *values_item = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(values_item);
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)metric_value;
    (void)values_count;
    return 66;
}