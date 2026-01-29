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
//<ID> 296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"device\",\"active\":false,\"meta\":{\"version\":\"1.0\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *new_name = cJSON_CreateString("new_device");
    char *printed_before = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(enabled_item);
    (void)root_is_obj;
    (void)enabled_true;
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);
    cJSON *replaced = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(replaced);
    (void)name_val;
    char *printed_after = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}