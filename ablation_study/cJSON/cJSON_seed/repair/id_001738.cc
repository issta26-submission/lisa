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
//<ID> 1738
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
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(config, "meta", raw);
    cJSON *flag = cJSON_AddBoolToObject(config, "enabled", 1);

    // step 2: Configure
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddStringToObject(info, "version", "v1.0");
    cJSON_AddItemToObject(info, "note", cJSON_CreateString("init"));

    // step 3: Operate and Validate
    char *name_val = cJSON_GetStringValue(name);
    char *raw_val = cJSON_GetStringValue(raw);
    cJSON_bool is_enabled = cJSON_IsTrue(flag);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = raw_val ? raw_val[0] : 'Z';
    buffer[3] = (char)('0' + is_enabled);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}