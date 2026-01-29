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
//<ID> 1077
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"metrics\":{\"value\":3.14},\"name\":\"sensor\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *metrics = cJSON_GetObjectItem(root, "metrics");
    double samples_vals[] = {0.1, 0.2, 0.3, 0.4};
    cJSON *samples = cJSON_CreateDoubleArray(samples_vals, 4);
    cJSON_AddItemToObject(metrics, "samples", samples);
    cJSON *new_number = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(metrics, "value", new_number);
    cJSON *active = cJSON_AddTrueToObject(root, "active");

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    (void)replaced;
    (void)active;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}