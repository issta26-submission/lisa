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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"old\",\"flag\":false,\"meta\":{\"tag\":\"v1\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *new_name = cJSON_CreateString("new");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);
    cJSON *new_flag = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(root, flag_item, new_flag);
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddStringToObject(new_meta, "tag", "v2");
    cJSON_ReplaceItemViaPointer(root, meta_item, new_meta);

    // step 3: Operate and Validate
    cJSON *name_after = cJSON_GetObjectItem(root, "name");
    cJSON *flag_after = cJSON_GetObjectItem(root, "flag");
    cJSON *meta_after = cJSON_GetObjectItem(root, "meta");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_after);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_after);
    char *name_str = cJSON_GetStringValue(name_after);
    char *out = cJSON_PrintUnformatted(root);
    (void)name_str;
    (void)flag_is_bool;
    (void)meta_is_obj;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}