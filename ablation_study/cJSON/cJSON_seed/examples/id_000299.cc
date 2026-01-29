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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *ver_item = cJSON_CreateString(cJSON_Version());
    cJSON *enabled = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(settings, "version", ver_item);
    cJSON_AddItemToObjectCS(settings, "enabled", enabled);
    cJSON_AddItemToObjectCS(root, "settings", settings);

    // step 3: Operate & Validate
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON *has_settings_item = cJSON_CreateBool(has_settings);
    cJSON_AddItemToObjectCS(root, "has_settings", has_settings_item);
    cJSON *got_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *got_version = cJSON_GetObjectItem(got_settings, "version");
    const char *version_text = cJSON_GetStringValue(got_version);
    cJSON_AddItemToObjectCS(root, "version_copy", cJSON_CreateString(version_text));
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), (cJSON_bool)1);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_AddItemToObjectCS(root, "printed_unformatted", cJSON_CreateString(printed));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "version_copy");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}