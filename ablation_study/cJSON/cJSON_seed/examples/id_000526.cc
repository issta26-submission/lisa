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
//<ID> 526
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
    cJSON_AddItemToObjectCS(config, "enabled", enabled);
    cJSON *name = cJSON_CreateString("demo-config");
    cJSON_AddItemToObjectCS(config, "name", name);
    cJSON_AddNumberToObject(root, "version", 3.14);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_config = cJSON_GetObjectItem(parsed, "config");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_config, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed_config, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(parsed_enabled);
    cJSON_bool original_true = cJSON_IsTrue(enabled);

    // step 3: Operate & Validate
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s;orig_enabled=%d;parsed_enabled=%d", name_str ? name_str : "null", (int)original_true, (int)enabled_true);
    char *parsed_snapshot = cJSON_Print(parsed);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s;equal=%d", name_str ? name_str : "null", (int)equal);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(parsed_snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}