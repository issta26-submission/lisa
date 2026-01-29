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
//<ID> 1900
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
    cJSON *name_added = cJSON_AddStringToObject(settings, "name", "example");
    cJSON *enabled_added = cJSON_AddFalseToObject(settings, "enabled");

    // step 2: Configure
    cJSON_bool has_root_settings = cJSON_HasObjectItem(root, "settings");
    cJSON *got_settings = cJSON_GetObjectItem(root, "settings");
    cJSON_bool has_enabled = cJSON_HasObjectItem(got_settings, "enabled");
    cJSON *got_enabled = cJSON_GetObjectItem(got_settings, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON *got_name = cJSON_GetObjectItem(got_settings, "name");
    const char *name_value = cJSON_GetStringValue(got_name);
    char *printed = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)(has_root_settings % 10));
    buffer[2] = (char)('0' + (int)(has_enabled % 10));
    buffer[3] = (char)('0' + (int)(enabled_is_bool % 10));
    buffer[4] = name_value ? name_value[0] : 'N';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}