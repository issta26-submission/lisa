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
//<ID> 499
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
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "cjson_demo");
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddItemToObjectCS(root, "meta_cs", meta);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 3: Operate & Validate
    char snapshot_buf_marker = 0;
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    sprintf(status_buf, "ver=%s;first=%s;equal=%d", version ? version : "unknown", first_str ? first_str : "null", (int)equal);
    (void)snapshot_buf_marker;
    (void)status_buf;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}