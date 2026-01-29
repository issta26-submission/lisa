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
//<ID> 498
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
    cJSON_AddStringToObject(root, "app", "cJSON_fuzzer");
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *items_cs = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items_cs", items_cs);

    // step 2: Configure
    cJSON *s_alpha = cJSON_CreateString("alpha");
    cJSON *s_beta = cJSON_CreateString("beta");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *t = cJSON_CreateTrue();
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, s_alpha);
    cJSON_AddItemToArray(items, s_beta);
    cJSON_AddItemToArray(items, num);
    cJSON_AddItemToArray(items, t);
    cJSON_AddItemToArray(items_cs, cJSON_CreateString("ref1"));
    cJSON_AddItemToArray(items_cs, f);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    int parsed_size = cJSON_GetArraySize(parsed_items);
    const char *first_item = cJSON_GetStringValue(cJSON_GetArrayItem(parsed_items, 0));
    char summary[128];
    memset(summary, 0, sizeof(summary));
    sprintf(summary, "ver=%s;first=%s;count=%d", version ? version : "unknown", first_item ? first_item : "null", parsed_size);
    cJSON_AddStringToObject(parsed, "summary", summary);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}