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
//<ID> 491
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
    cJSON *items = cJSON_CreateArray();
    cJSON *cs_items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObjectCS(root, "cs_items", cs_items);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "fuzzer");
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddStringToObject(root, "version", version ? version : "unknown");
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON *cs0 = cJSON_CreateString("cs_one");
    cJSON_AddItemToArray(cs_items, cs0);

    // step 3: Operate & Validate
    char summary[256];
    memset(summary, 0, sizeof(summary));
    const char *first = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    const char *second = cJSON_GetStringValue(cJSON_GetArrayItem(items, 1));
    const char *ver = cJSON_GetStringValue(cJSON_GetObjectItem(root, "version"));
    sprintf(summary, "first=%s;second=%s;ver=%s", first ? first : "null", second ? second : "null", ver ? ver : "null");
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