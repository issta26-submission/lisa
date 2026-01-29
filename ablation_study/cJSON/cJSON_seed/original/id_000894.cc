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
//<ID> 894
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
    double dbls[3] = {1.5, 2.5, 3.5};
    float flts[2] = {10.0f, 20.0f};
    cJSON *darr = cJSON_CreateDoubleArray(dbls, 3);
    cJSON *farr = cJSON_CreateFloatArray(flts, 2);
    cJSON_AddItemToObject(cfg, "double_array", darr);
    cJSON_AddItemToObject(cfg, "float_array", farr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(cfg, "meta", meta);
    cJSON_AddStringToObject(meta, "source", "sensor_A");

    // step 3: Operate
    cJSON *d0 = cJSON_GetArrayItem(darr, 0);
    double v0 = cJSON_GetNumberValue(d0);
    cJSON *f1 = cJSON_GetArrayItem(farr, 1);
    double v1 = cJSON_GetNumberValue(f1);
    double combined = v0 + v1;
    cJSON_AddNumberToObject(meta, "combined", combined);
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(cfg, "double_array");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}