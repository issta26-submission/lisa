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
//<ID> 905
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddTrueToObject(settings, "enabled");
    cJSON *name_ref = cJSON_CreateStringReference("device-xyz");
    cJSON_AddItemToObject(settings, "name", name_ref);
    double dvals[3] = {1.1, 2.2, 3.3};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(settings, "thresholds", darr);

    // step 3: Operate
    cJSON *t0 = cJSON_GetArrayItem(darr, 0);
    cJSON *t1 = cJSON_GetArrayItem(darr, 1);
    double v0 = cJSON_GetNumberValue(t0);
    double v1 = cJSON_GetNumberValue(t1);
    double sum12 = v0 + v1;
    cJSON_AddNumberToObject(settings, "sum12", sum12);
    cJSON *new_sum = cJSON_CreateNumber(sum12 + 0.5);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(settings, "sum12", new_sum);
    (void)replaced;
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    char *prebuf = (char *)buf;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    (void)pre_ok;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);
    (void)enabled_flag;

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}