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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"device\":\"sensor\",\"metrics\":[1,2,3],\"active\":true}";
    cJSON *root = cJSON_Parse(json);
    cJSON *metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *device = cJSON_GetObjectItem(root, "device");
    cJSON *active = cJSON_GetObjectItem(root, "active");

    // step 2: Configure
    cJSON *new_metrics = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(new_metrics, n1);
    cJSON_AddItemToArray(new_metrics, n2);
    cJSON_AddItemToArray(new_metrics, n3);
    cJSON *device_new = cJSON_CreateString("sensor-v2");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "metrics", new_metrics);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "device", device_new);

    // step 3: Operate and Validate
    cJSON *metrics_after = cJSON_GetObjectItem(root, "metrics");
    cJSON *first = cJSON_GetArrayItem(metrics_after, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *computed = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToArray(metrics_after, computed);
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}