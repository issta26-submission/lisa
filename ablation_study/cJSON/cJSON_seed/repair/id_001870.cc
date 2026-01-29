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
//<ID> 1870
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
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(flags, "enabled", enabled);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(settings, "name", name);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(settings, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON *settings_check = cJSON_GetObjectItem(root, "settings");
    cJSON_bool settings_is_object = cJSON_IsObject(settings_check);
    char *set_ret = cJSON_SetValuestring(name_item, "example_v2");

    // step 3: Operate and Validate
    cJSON *meta = cJSON_AddObjectToObject(settings, "meta");
    cJSON_AddItemToObject(meta, "version", cJSON_CreateNumber(2.0));
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char *name_str = cJSON_GetStringValue(name_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)(name_is_string % 10));
    buffer[2] = (char)('0' + (int)(settings_is_object % 10));
    buffer[3] = set_ret ? set_ret[0] : 'S';
    buffer[4] = name_str ? name_str[0] : 'N';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}