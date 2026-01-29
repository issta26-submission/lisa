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
//<ID> 692
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *num_pi = cJSON_CreateNumber(3.14159);
    cJSON *label = cJSON_CreateString("example");

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddBoolToObject(settings, "enabled", 1);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(root, "pi", num_pi);
    cJSON_AddItemToObject(root, "label", label);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 0);
    cJSON *settings_copy = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_copy);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_settings = cJSON_GetObjectItemCaseSensitive(parsed, "settings");
    cJSON *parsed_enabled = cJSON_GetObjectItemCaseSensitive(parsed_settings, "enabled");
    cJSON_bool parsed_is_true = cJSON_IsTrue(parsed_enabled);
    cJSON_bool equal_settings = cJSON_Compare(settings, parsed_settings, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "active");
    (void)version;
    (void)enabled_item;
    (void)active_item;
    (void)parsed_is_true;
    (void)equal_settings;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}