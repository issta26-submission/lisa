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
//<ID> 907
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
    double dvals[3] = {1.0, 2.5, 3.5};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(cfg, "doubles", darr);
    cJSON *enabled = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *label_ref = cJSON_CreateStringReference("initial");
    cJSON_AddItemToObject(cfg, "label", label_ref);

    // step 2: Configure
    cJSON *d0 = cJSON_GetArrayItem(darr, 0);
    cJSON *d1 = cJSON_GetArrayItem(darr, 1);
    cJSON *d2 = cJSON_GetArrayItem(darr, 2);
    double v0 = cJSON_GetNumberValue(d0);
    double v1 = cJSON_GetNumberValue(d1);
    double v2 = cJSON_GetNumberValue(d2);
    int flag = (int)cJSON_IsTrue(enabled);
    double total = v0 + v1 + v2 + (double)flag;
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats, "total", total);
    cJSON_AddItemToObject(root, "stats", stats);

    // step 3: Operate
    cJSON *new_label = cJSON_CreateStringReference("updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(cfg, "label", new_label);
    (void)replaced;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    void *buf = cJSON_malloc(128);
    memset(buf, 0, 128);
    char *prebuf = (char *)buf;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 128, 0);
    (void)pre_ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}