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
//<ID> 42
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
    cJSON *name_str = cJSON_CreateString("deviceA");
    cJSON_AddItemToObject(settings, "name", name_str);

    // step 2: Configure
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled);
    cJSON *mode_str = cJSON_CreateString("auto");
    cJSON_AddItemToObject(settings, "mode", mode_str);

    // step 3: Operate and Validate
    cJSON *retrieved_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *retrieved_name = cJSON_GetObjectItem(retrieved_settings, "name");
    const char *name_val = cJSON_GetStringValue(retrieved_name);
    cJSON *retrieved_enabled = cJSON_GetObjectItem(retrieved_settings, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(retrieved_enabled);
    int validation_value = (int)is_enabled + (int)(name_val != NULL);
    cJSON_AddNumberToObject(root, "validation", (double)validation_value);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}