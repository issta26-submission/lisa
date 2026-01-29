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
//<ID> 992
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *null_marker = cJSON_AddNullToObject(root, "unset");

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddBoolToObject(settings, "enabled", 1);
    cJSON *count_item = cJSON_CreateNumber(5.0);
    cJSON_AddItemToObject(settings, "count", count_item);
    cJSON *name_item = cJSON_CreateString("example_project");
    cJSON_AddItemToObject(settings, "name", name_item);

    // step 3: Operate and Validate
    cJSON *settings_ref = cJSON_GetObjectItem(root, "settings");
    cJSON *enabled_cs = cJSON_GetObjectItemCaseSensitive(settings_ref, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_cs);
    cJSON_AddBoolToObject(root, "enabled_copy", is_enabled);
    double count_val_before = cJSON_GetNumberValue(count_item);
    cJSON_SetNumberHelper(count_item, count_val_before + 2.0);
    double count_val_after = cJSON_GetNumberValue(count_item);
    (void)count_val_before;
    (void)count_val_after;
    (void)null_marker;
    (void)meta;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}