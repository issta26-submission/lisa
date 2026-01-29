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
//<ID> 525
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

    // step 2: Configure
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(config, "enabled", enabled);
    cJSON *msg = cJSON_CreateString("hello-world");
    cJSON_AddItemToObjectCS(config, "msg", msg);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_config = cJSON_GetObjectItem(parsed, "config");
    cJSON *parsed_msg = cJSON_GetObjectItem(parsed_config, "msg");
    const char *msg_str = cJSON_GetStringValue(parsed_msg);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed_config, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(parsed_enabled);
    char status[128];
    memset(status, 0, sizeof(status));
    sprintf(status, "msg=%s;enabled=%d", msg_str ? msg_str : "null", (int)enabled_true);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}