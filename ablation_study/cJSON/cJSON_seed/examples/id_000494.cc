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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", version ? version : "unknown");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "active");
    cJSON_AddBoolToObject(meta, "enabled", 1);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    char info[128];
    memset(info, 0, sizeof(info));
    sprintf(info, "first=%s;ver=%s", first_str ? first_str : "null", version ? version : "unknown");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_items);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}