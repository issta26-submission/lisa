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
//<ID> 502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *username_item = cJSON_CreateString("bob@example.com");
    cJSON_AddItemToObject(root, "username", username_item);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);
    cJSON *raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "extra", raw_item);

    // step 2: Configure
    cJSON *fetched_user = cJSON_GetObjectItem(root, "username");
    cJSON *fetched_active = cJSON_GetObjectItem(root, "active");
    cJSON *fetched_raw = cJSON_GetObjectItem(root, "extra");
    char *user_str = cJSON_GetStringValue(fetched_user);
    cJSON_bool active_is_true = cJSON_IsTrue(fetched_active);
    cJSON_bool extra_is_raw = cJSON_IsRaw(fetched_raw);
    cJSON *owner_copy = cJSON_CreateString(user_str);
    cJSON_AddItemToObject(root, "owner", owner_copy);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = (size_t)128;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    char *dup_owner_str = cJSON_GetStringValue(cJSON_GetObjectItem(root_dup, "owner"));

    // step 4: Validate & Cleanup
    (void)dup_owner_str;
    (void)active_is_true;
    (void)extra_is_raw;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}