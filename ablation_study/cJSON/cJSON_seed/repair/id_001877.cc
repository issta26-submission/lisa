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
//<ID> 1877
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
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "active", active);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "example_v2");
    (void)set_ret;
    cJSON_bool is_root_obj = cJSON_IsObject(root);
    cJSON_bool is_settings_obj = cJSON_IsObject(settings);
    cJSON_bool is_name_string = cJSON_IsString(name);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (is_name_string % 10));
    buffer[2] = (char)('0' + (is_root_obj % 10));
    buffer[3] = (char)('0' + (is_settings_obj % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}