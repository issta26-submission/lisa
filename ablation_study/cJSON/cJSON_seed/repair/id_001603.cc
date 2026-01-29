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
//<ID> 1603
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
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled_true);
    cJSON *visible_false = cJSON_CreateBool(0);
    cJSON_AddItemToObject(settings, "visible", visible_false);
    cJSON *explicit_true = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "explicit_true", explicit_true);

    // step 3: Operate and Validate
    char *json_unformatted = cJSON_PrintUnformatted(root);
    const char *version = cJSON_Version();
    cJSON *got_settings = cJSON_GetObjectItemCaseSensitive(root, "settings");
    cJSON *got_enabled = cJSON_GetObjectItemCaseSensitive(got_settings, "enabled");
    cJSON_bool enabled_check = cJSON_IsTrue(got_enabled);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = version[0];
    buffer[1] = json_unformatted[0];
    buffer[2] = (char)('0' + (int)enabled_check);

    // step 4: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}