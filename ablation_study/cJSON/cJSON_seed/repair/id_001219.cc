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
//<ID> 1219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *base = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(meta, "base", base);

    // step 2: Configure
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "dup_meta", dup_meta);
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddItemToObject(dup_meta, "extra", extra);
    cJSON *exnum = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(extra, "exnum", exnum);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "dup_meta");
    cJSON_AddItemToArray(items, detached);
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it1 = cJSON_GetArrayItem(items, 1);
    cJSON *meta_base = cJSON_GetObjectItem(meta, "base");
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double vbase = cJSON_GetNumberValue(meta_base);
    double total = v0 + v1 + vbase;
    cJSON_AddNumberToObject(root, "final", total);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}