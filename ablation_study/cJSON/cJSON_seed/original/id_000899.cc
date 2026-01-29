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
//<ID> 899
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
    float fvals[3] = {1.5f, 2.5f, 3.25f};
    double dvals[3] = {4.0, 5.5, 6.75};
    cJSON *farr = cJSON_CreateFloatArray(fvals, 3);
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(cfg, "floats", farr);
    cJSON_AddItemToObject(cfg, "doubles", darr);
    cJSON_AddNumberToObject(cfg, "count", 3);

    // step 2: Configure
    cJSON *f_item0 = cJSON_GetArrayItem(farr, 0);
    cJSON *f_item1 = cJSON_GetArrayItem(farr, 1);
    cJSON *d_item0 = cJSON_GetArrayItem(darr, 0);
    double f0 = cJSON_GetNumberValue(f_item0);
    double f1 = cJSON_GetNumberValue(f_item1);
    double d0 = cJSON_GetNumberValue(d_item0);
    double total = f0 + f1 + d0;
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats, "total", total);
    cJSON_AddItemToObject(root, "stats", stats);

    // step 3: Operate
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 256, 1);
    (void)pre_ok;
    char *out = cJSON_PrintUnformatted(root);
    cJSON *retrieved_cfg = cJSON_GetObjectItem(root, "config");
    cJSON_DeleteItemFromObject(retrieved_cfg, "floats");

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}