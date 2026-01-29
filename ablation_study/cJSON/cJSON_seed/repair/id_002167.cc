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
//<ID> 2167
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
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *name = cJSON_CreateString("device_123");
    cJSON_AddItemToObject(settings, "name", name);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(settings, "enabled", enabled);
    cJSON *version = cJSON_CreateNumber(1.2);
    cJSON_AddItemToObject(settings, "version", version);

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "values", values);
    cJSON *v1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(values, v1);
    cJSON *v2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(values, v2);
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);

    // step 3: Operate and Validate
    cJSON *settings_get = cJSON_GetObjectItemCaseSensitive(root, "settings");
    cJSON *name_get = cJSON_GetObjectItemCaseSensitive(settings_get, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_get);
    cJSON *enabled_get = cJSON_GetObjectItemCaseSensitive(settings_get, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_get);
    cJSON *flag_name = cJSON_CreateBool(name_is_str);
    cJSON_AddItemToObject(root, "name_is_string", flag_name);
    cJSON *flag_enabled = cJSON_CreateBool(enabled_is_bool);
    cJSON_AddItemToObject(root, "enabled_is_bool", flag_enabled);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json[0];
    buffer[1] = json[1];
    buffer[2] = json[2];
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}