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
//<ID> 900
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    double nums[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *darr = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(cfg, "values", darr);

    // step 2: Configure
    cJSON *enabled = cJSON_AddTrueToObject(cfg, "enabled");
    const char *device_name = "sensorA";
    cJSON *name_ref = cJSON_CreateStringReference(device_name);
    cJSON_AddItemToObject(cfg, "name", name_ref);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "initializing");

    // step 3: Operate
    cJSON *d0 = cJSON_GetArrayItem(darr, 0);
    cJSON *d1 = cJSON_GetArrayItem(darr, 1);
    double v0 = cJSON_GetNumberValue(d0);
    double v1 = cJSON_GetNumberValue(d1);
    double partial = v0 + v1;
    cJSON_AddNumberToObject(cfg, "partial_sum", partial);
    cJSON *ready_ref = cJSON_CreateStringReference("ready");
    cJSON_ReplaceItemInObjectCaseSensitive(meta, "status", ready_ref);

    // step 4: Validate & Cleanup
    cJSON *status = cJSON_GetObjectItem(meta, "status");
    const char *status_str = cJSON_GetStringValue(status);
    cJSON_AddStringToObject(cfg, "final_status", status_str);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}