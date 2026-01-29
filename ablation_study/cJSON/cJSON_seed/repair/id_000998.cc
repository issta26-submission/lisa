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
//<ID> 998
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

    // step 2: Configure
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddNullToObject(root, "optional");
    cJSON_AddBoolToObject(meta, "enabled", 0);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *snap_ref = cJSON_CreateStringReference(buffer);
    cJSON_AddItemToObject(root, "snapshot", snap_ref);

    // step 3: Operate and Validate
    cJSON *root_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON *root_enabled_cs = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON *root_snapshot = cJSON_GetObjectItem(root, "snapshot");
    const char *snapshot_value = cJSON_GetStringValue(root_snapshot);
    cJSON *meta_obj = cJSON_GetObjectItem(root, "meta");
    cJSON *meta_enabled_item = cJSON_GetObjectItem(meta_obj, "enabled");
    cJSON_bool flag_root = cJSON_IsTrue(root_enabled);
    cJSON_bool flag_root_cs = cJSON_IsTrue(root_enabled_cs);
    cJSON_bool flag_meta = cJSON_IsTrue(meta_enabled_item);
    (void)snapshot_value;
    (void)flag_root;
    (void)flag_root_cs;
    (void)flag_meta;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}