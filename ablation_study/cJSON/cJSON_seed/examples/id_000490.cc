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
//<ID> 490
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
    cJSON_AddStringToObject(root, "tool", "fuzzer");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *s_a = cJSON_CreateString("alpha");
    cJSON *s_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s_a);
    cJSON_AddItemToArray(items, s_b);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", version ? version : "unknown");

    // step 3: Operate & Validate
    char info[256];
    memset(info, 0, sizeof(info));
    int count = cJSON_GetArraySize(items);
    const char *first_val = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    sprintf(info, "ver=%s;count=%d;first=%s", version ? version : "none", count, first_val ? first_val : "null");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_Minify(snapshot);
    cJSON *parsed = cJSON_ParseWithOpts(snapshot, NULL, 1);
    cJSON_bool same = cJSON_Compare(root, parsed, 1);
    (void)same;
    (void)info;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}