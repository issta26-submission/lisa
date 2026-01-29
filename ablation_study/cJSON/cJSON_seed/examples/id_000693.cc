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
//<ID> 693
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
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *enabled = cJSON_AddBoolToObject(settings, "enabled", 1);

    // step 2: Configure
    cJSON *enabled_dup = cJSON_Duplicate(enabled, 1);
    cJSON_AddItemToArray(flags, enabled_dup);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, flag_false);
    cJSON *note = cJSON_CreateString("cfg");
    cJSON_AddItemToObject(root, "note", note);

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
    cJSON_bool equal = cJSON_Compare(enabled, parsed_enabled, 1);
    (void)version;
    (void)equal;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "settings");
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}