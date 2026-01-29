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
//<ID> 1871
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", name);

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(config, "settings");
    cJSON_AddItemToObject(settings, "featureA", cJSON_CreateTrue());
    cJSON_AddItemToObject(settings, "featureB", cJSON_CreateTrue());

    // step 3: Operate and Validate
    cJSON_bool is_cfg_obj = cJSON_IsObject(config);
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    cJSON_bool is_name_str = cJSON_IsString(name_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (is_cfg_obj ? 1 : 0));
    buffer[2] = (char)('0' + (is_name_str ? 1 : 0));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}