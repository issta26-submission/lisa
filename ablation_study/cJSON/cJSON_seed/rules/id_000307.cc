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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *enabled = cJSON_CreateTrue();
    cJSON *message = cJSON_CreateString("hello world");
    char *printed = NULL;
    char *prebuf = (char *)cJSON_malloc(256);
    memset(prebuf, 0, 256);
    cJSON_bool is_enabled = 0;
    const char *msg_str = NULL;
    cJSON *retrieved_settings = NULL;
    cJSON *retrieved_enabled = NULL;
    cJSON *retrieved_message = NULL;
    cJSON_bool pre_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "enabled", enabled);
    cJSON_AddItemToObject(root, "message", message);

    // step 3: Operate / Validate
    retrieved_settings = cJSON_GetObjectItem(root, "settings");
    retrieved_enabled = cJSON_GetObjectItem(retrieved_settings, "enabled");
    is_enabled = cJSON_IsBool(retrieved_enabled);
    retrieved_message = cJSON_GetObjectItem(root, "message");
    msg_str = cJSON_GetStringValue(retrieved_message);
    printed = cJSON_PrintUnformatted(root);
    pre_ok = cJSON_PrintPreallocated(root, prebuf, 256, 0);
    prebuf[0] = (char)('0' + (int)is_enabled);
    prebuf[1] = msg_str ? msg_str[0] : '\0';
    (void)pre_ok;
    (void)printed;
    (void)msg_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}