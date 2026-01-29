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
//<ID> 909
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
    cJSON *enabled = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *name_ref = cJSON_CreateStringReference("original-name");
    cJSON_AddItemToObject(cfg, "name", name_ref);
    double values_arr[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *darr = cJSON_CreateDoubleArray(values_arr, 4);
    cJSON_AddItemToObject(cfg, "values", darr);

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(darr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats, "first", first_val);
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON *new_name = cJSON_CreateStringReference("replaced-name");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(cfg, "name", new_name);
    (void)replaced;

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(root, prebuf, 256, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}