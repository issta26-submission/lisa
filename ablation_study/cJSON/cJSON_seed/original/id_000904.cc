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
//<ID> 904
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

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddTrueToObject(cfg, "enabled");
    const char *mode_literal = "fast";
    cJSON *mode_ref = cJSON_CreateStringReference(mode_literal);
    cJSON_AddItemToObject(cfg, "mode", mode_ref);
    double dvals[3] = {1.1, 2.2, 3.3};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(cfg, "values", darr);

    // step 3: Operate
    cJSON *replacement_mode = cJSON_CreateStringReference("slow");
    cJSON_bool replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(cfg, "mode", replacement_mode);
    (void)replace_ok;
    cJSON *a0 = cJSON_GetArrayItem(darr, 0);
    cJSON *a1 = cJSON_GetArrayItem(darr, 1);
    cJSON *a2 = cJSON_GetArrayItem(darr, 2);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double total = v0 + v1 + v2;
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats, "total", total);
    cJSON_AddItemToObject(root, "stats", stats);
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    char *prebuf = (char *)buf;
    cJSON_PrintPreallocated(root, prebuf, 512, 1);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}