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
//<ID> 509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_name[] = "Device42";
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString(initial_name);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 0);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 2: Configure
    cJSON_bool active_is_true_before = cJSON_IsTrue(active_item);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *new_active = cJSON_CreateTrue();
    cJSON_ReplaceItemInObject(root, "active", new_active);
    cJSON *active_after = cJSON_GetObjectItem(root, "active");

    // step 3: Operate
    size_t buf_len = 64 + (size_t)(raw_is_raw) + (name_str ? strlen(name_str) : 0);
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_bool active_is_true_after = cJSON_IsTrue(active_after);
    char *extracted_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));

    // step 4: Validate & Cleanup
    (void)active_is_true_before;
    (void)active_is_true_after;
    (void)raw_is_raw;
    (void)extracted_name;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}