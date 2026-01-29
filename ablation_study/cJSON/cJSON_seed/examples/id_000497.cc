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
//<ID> 497
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
    cJSON_AddStringToObject(root, "name", "fuzzer");
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToArray(items, num);
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddBoolToObject(meta, "valid", 1);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    const char *first = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    double value = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 2));
    char info[256];
    memset(info, 0, sizeof(info));
    sprintf(info, "ver=%s;first=%s;num=%.0f;eq=%d", version ? version : "unknown", first ? first : "null", value, (int)equal);
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_items);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}