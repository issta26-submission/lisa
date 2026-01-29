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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "old");
    cJSON_AddFalseToObject(root, "active");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "tag", "v1");

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_name = cJSON_CreateString("new");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddStringToObject(new_meta, "tag", "v2");

    // step 3: Operate and Validate
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_bool = cJSON_IsBool(active_item);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_item);
    cJSON_AddNumberToObject(root, "active_flag", (double)active_is_bool);
    cJSON_ReplaceItemViaPointer(root, meta_item, new_meta);
    cJSON *name_after = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_after);
    (void)name_str;
    cJSON *final_meta = cJSON_GetObjectItem(root, "meta");
    (void)meta_is_obj;
    (void)active_is_bool;
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}