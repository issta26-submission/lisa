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
//<ID> 2269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "threshold", cJSON_CreateNumber(42.0));
    cJSON_AddItemToObject(settings, "name", cJSON_CreateString("sensor-A"));
    cJSON_AddItemToObject(config, "settings", settings);
    cJSON *settings_copy = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_copy);

    // step 2: Configure
    const char *version = cJSON_Version();
    cJSON *ver_node = cJSON_CreateString(version ? version : "unknown");
    cJSON_AddItemToObject(root, "version", ver_node);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (version && version[0]) ? version[0] : '\0';

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("settings-replaced");
    cJSON_ReplaceItemViaPointer(config, settings, replacement);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}