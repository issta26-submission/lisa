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
//<ID> 500
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
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    cJSON *found_items = cJSON_GetObjectItem(root, "items");
    const char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(found_items, 0));
    sprintf(status_buf, "first=%s;copied=%d", first_str ? first_str : "null", (int)cJSON_GetArraySize(items_copy));

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    cJSON *found_meta = cJSON_GetObjectItem(parsed, "meta");
    char *printed_meta = cJSON_Print(found_meta);
    cJSON_free(printed_meta);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}