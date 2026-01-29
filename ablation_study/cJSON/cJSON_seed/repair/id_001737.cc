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
//<ID> 1737
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

    // step 2: Configure
    cJSON *mode_item = cJSON_CreateString("production");
    cJSON_AddItemToObject(settings, "mode", mode_item);
    cJSON *raw_cfg = cJSON_CreateRaw("{\"threshold\":10}");
    cJSON_AddItemToObject(settings, "rawcfg", raw_cfg);
    cJSON *enabled_item = cJSON_AddBoolToObject(settings, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *retr_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *retr_mode = cJSON_GetObjectItem(retr_settings, "mode");
    const char *mode_val = cJSON_GetStringValue(retr_mode);
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = mode_val ? mode_val[0] : 'Z';
    buffer[2] = (char)('0' + (is_enabled ? 1 : 0));
    (void)buffer;
    (void)mode_val;
    (void)is_enabled;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}