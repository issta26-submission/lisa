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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"old\",\"active\":false,\"meta\":{\"tag\":\"v1\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_name = cJSON_CreateString("new");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    double active_flag_val = (double)cJSON_IsBool(active_item);
    cJSON_AddNumberToObject(root, "active_flag", active_flag_val);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_item);

    // step 3: Operate and Validate
    cJSON *name_after = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_after);
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddStringToObject(new_meta, "tag", "v2");
    cJSON_ReplaceItemViaPointer(root, meta_item, new_meta);
    cJSON *meta_after = cJSON_GetObjectItem(root, "meta");
    cJSON *tag_item = cJSON_GetObjectItem(meta_after, "tag");
    const char *tag_str = cJSON_GetStringValue(tag_item);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *formatted = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(formatted);
    cJSON_Delete(root);
    (void)name_str;
    (void)tag_str;
    (void)meta_is_obj;

    // API sequence test completed successfully
    return 66;
}