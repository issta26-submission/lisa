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
//<ID> 463
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

    // step 2: Configure
    cJSON *name_item = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *ver_item = cJSON_CreateNumber(2026);
    cJSON_AddItemToObject(meta, "version", ver_item);
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, item_a);
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, item_b);

    // step 3: Operate & Validate
    cJSON *meta_retrieved = cJSON_GetObjectItem(root, "meta");
    cJSON *name_retrieved = cJSON_GetObjectItem(meta_retrieved, "name");
    const char *name_value = cJSON_GetStringValue(name_retrieved);
    char info_buf[128];
    memset(info_buf, 0, sizeof(info_buf));
    sprintf(info_buf, "before=%s|ver=%.0f", name_value, cJSON_GetNumberValue(cJSON_GetObjectItem(meta_retrieved, "version")));
    cJSON *new_name = cJSON_CreateString("replaced_name");
    cJSON_ReplaceItemViaPointer(meta_retrieved, name_retrieved, new_name);
    const char *name_after = cJSON_GetStringValue(cJSON_GetObjectItem(meta_retrieved, "name"));
    char confirm_buf[128];
    memset(confirm_buf, 0, sizeof(confirm_buf));
    sprintf(confirm_buf, "after=%s", name_after);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}