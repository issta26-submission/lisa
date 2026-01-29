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
//<ID> 503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("DeviceX");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddRawToObject(root, "payload", "{\"bin\":true}");
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON *payload_item = cJSON_GetObjectItem(root, "payload");

    // step 2: Configure
    char *name_str = cJSON_GetStringValue(name);
    cJSON *owner_ref = cJSON_CreateStringReference(name_str);
    cJSON_AddItemToObject(root, "owner", owner_ref);
    cJSON_bool active_true = cJSON_IsTrue(active_item);
    cJSON_bool payload_is_raw = cJSON_IsRaw(payload_item);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 128 + (size_t)strlen(name_str) + (size_t)active_true + (size_t)payload_is_raw;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_ReplaceItemInObject(root, "owner", cJSON_CreateString("NewOwner"));
    cJSON *owner_item = cJSON_GetObjectItem(root, "owner");
    char *owner_str = cJSON_GetStringValue(owner_item);

    // step 4: Validate & Cleanup
    (void)owner_str;
    (void)name_str;
    (void)active_true;
    (void)payload_is_raw;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}