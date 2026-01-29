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
//<ID> 994
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
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNullToObject(meta, "nullable");
    cJSON_AddBoolToObject(settings, "enabled", 1);

    // step 2: Configure
    cJSON_AddStringToObject(settings, "name", "example_project");
    cJSON_AddNumberToObject(meta, "id", 101.0);
    cJSON *entries = cJSON_AddArrayToObject(root, "entries");
    cJSON *first = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(entries, first);
    cJSON *second = cJSON_CreateString("beta");
    cJSON_AddItemToArray(entries, second);

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *settings_item = cJSON_GetObjectItem(root, "settings");
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(settings_item, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);
    cJSON *name_item = cJSON_GetObjectItem(settings_item, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON *meta_id_item = cJSON_GetObjectItemCaseSensitive(meta, "id");
    double id_val = cJSON_GetNumberValue(meta_id_item);
    cJSON *summary = cJSON_AddObjectToObject(root, "summary");
    cJSON_AddStringToObject(summary, "name_copy", name_val);
    cJSON_AddNumberToObject(summary, "id_copy", id_val);
    cJSON_AddBoolToObject(summary, "enabled_copy", enabled_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}