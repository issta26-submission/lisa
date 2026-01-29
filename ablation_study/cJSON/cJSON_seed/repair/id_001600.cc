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
//<ID> 1600
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    const char *version = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(version);
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "enabled", true_item);
    cJSON *false_item = cJSON_CreateBool((cJSON_bool)0);
    cJSON_AddItemToObject(child, "active", false_item);

    // step 3: Operate and Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    cJSON *got_ver = cJSON_GetObjectItem(root, "version");
    cJSON *got_enabled = cJSON_GetObjectItem(child, "enabled");
    cJSON *got_active = cJSON_GetObjectItem(child, "active");
    const char *got_ver_str = cJSON_GetStringValue(got_ver);
    cJSON_bool enabled_flag = cJSON_IsTrue(got_enabled);
    cJSON_bool active_flag = cJSON_IsTrue(got_active);
    buffer[0] = got_ver_str[0];
    buffer[1] = (char)('0' + (int)enabled_flag);
    buffer[2] = (char)('0' + (int)active_flag);
    buffer[3] = unformatted[0];

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}