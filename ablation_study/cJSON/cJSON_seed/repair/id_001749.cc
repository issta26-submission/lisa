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
//<ID> 1749
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
    cJSON *mode = cJSON_CreateString("auto");
    cJSON_AddItemToObject(settings, "mode", mode);
    cJSON *value = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(settings, "value", value);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(2.0));

    // step 2: Configure
    cJSON_AddStringToObject(root, "profile", "default");
    cJSON_AddStringToObject(root, "cjson_version", cJSON_Version());

    // step 3: Operate and Validate
    cJSON *settings_ref = cJSON_GetObjectItem(root, "settings");
    cJSON *mode_ref = cJSON_GetObjectItem(settings_ref, "mode");
    cJSON_bool is_invalid = cJSON_IsInvalid(mode_ref);
    cJSON *detached_mode = cJSON_DetachItemViaPointer(settings_ref, mode_ref);
    cJSON *detached_value = cJSON_DetachItemFromObject(settings_ref, "value");
    char *printed = cJSON_PrintUnformatted(root);
    const char *ver = cJSON_Version();
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char *mode_str = cJSON_GetStringValue(detached_mode);
    double val_num = cJSON_GetNumberValue(detached_value);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver ? ver[0] : 'Y';
    buffer[2] = (char)('0' + (int)is_invalid);
    buffer[3] = mode_str ? mode_str[0] : 'Z';
    buffer[4] = (char)('0' + ((int)val_num % 10));
    (void)buffer;
    (void)val_num;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_mode);
    cJSON_Delete(detached_value);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}