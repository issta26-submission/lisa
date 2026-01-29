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
//<ID> 493
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
    cJSON_AddStringToObject(root, "app", "cjson_test");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddBoolToObject(meta, "enabled", 1);
    cJSON_AddStringToObject(meta, "version", version ? version : "unknown");

    // step 3: Operate & Validate
    char info[256];
    memset(info, 0, sizeof(info));
    const char *first = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    sprintf(info, "ver=%s;first=%s", version ? version : "null", first ? first : "null");
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
    return 66;
    // API sequence test completed successfully
}