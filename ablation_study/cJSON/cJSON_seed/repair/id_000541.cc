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
//<ID> 541
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *name = cJSON_CreateString("device1");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "count", count);

    // step 2: Configure
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(meta, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToArray(items, name_copy);
    cJSON *detached_count = cJSON_DetachItemFromObject(meta, "count");
    cJSON_AddItemToArray(items, detached_count);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *dup_meta = cJSON_GetObjectItemCaseSensitive(root_dup, "meta");
    cJSON *dup_name = cJSON_GetObjectItemCaseSensitive(dup_meta, "name");
    char *dup_name_val = cJSON_GetStringValue(dup_name);
    cJSON *dup_name_copy = cJSON_CreateString(dup_name_val);
    cJSON_AddItemToObject(root_dup, "dup_name_copy", dup_name_copy);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}