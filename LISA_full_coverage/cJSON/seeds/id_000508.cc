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
//<ID> 508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *payload_raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "payload", payload_raw);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *dup_name = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "owner", dup_name);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_true = cJSON_IsTrue(active_item);
    cJSON *active_num = cJSON_CreateNumber((double)active_true);
    cJSON_AddItemToObject(root, "active_numeric", active_num);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 128;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool payload_is_raw = cJSON_IsRaw(payload_raw);
    cJSON *payload_raw_flag = cJSON_CreateBool(payload_is_raw);
    cJSON_AddItemToObject(root, "payload_is_raw", payload_raw_flag);

    // step 4: Validate & Cleanup
    char *owner_str = cJSON_GetStringValue(dup_name);
    cJSON *owner_ref = cJSON_CreateStringReference(owner_str);
    cJSON_AddItemToObject(root, "owner_ref", owner_ref);
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}