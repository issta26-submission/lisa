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
//<ID> 906
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
    double dvals[3] = {1.5, 2.5, 3.0};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(cfg, "values", darr);
    cJSON *enabled_item = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *id_ref = cJSON_CreateStringReference("original-id");
    cJSON_AddItemToObject(cfg, "id", id_ref);
    cJSON_AddNumberToObject(cfg, "count", 3);

    // step 3: Operate
    cJSON *v0 = cJSON_GetArrayItem(darr, 0);
    cJSON *v1 = cJSON_GetArrayItem(darr, 1);
    cJSON *v2 = cJSON_GetArrayItem(darr, 2);
    double sum = cJSON_GetNumberValue(v0) + cJSON_GetNumberValue(v1) + cJSON_GetNumberValue(v2);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats, "total", sum);
    cJSON_AddItemToObject(root, "stats", stats);
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    char *prebuf = (char *)buf;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    (void)pre_ok;
    cJSON *new_id = cJSON_CreateStringReference("replaced-id");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(cfg, "id", new_id);
    (void)replaced;

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}