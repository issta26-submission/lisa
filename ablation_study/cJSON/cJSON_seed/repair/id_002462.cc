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
//<ID> 2462
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
    cJSON *auto_item = cJSON_AddTrueToObject(settings, "auto_mode");
    cJSON *manual_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(settings, "manual_mode", manual_item);
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    char *printed_root = cJSON_Print(root);
    char *printed_settings = cJSON_Print(settings);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed_root && printed_root[0] ? printed_root[0] : '\0';
    buffer[1] = printed_settings && printed_settings[0] ? printed_settings[0] : '\0';

    // step 3: Operate
    cJSON_bool manual_is_false = cJSON_IsFalse(manual_item);
    buffer[2] = (char)(int)manual_is_false;
    cJSON *status = cJSON_CreateObject();
    cJSON_AddTrueToObject(status, "ok");
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Validate and Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_settings);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}