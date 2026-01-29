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
//<ID> 521
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
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "metadataCS", metadata);

    // step 2: Configure
    cJSON_AddStringToObject(metadata, "name", "example-name");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_settings = cJSON_GetObjectItem(parsed, "settings");
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed_settings, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(parsed_enabled);
    cJSON *parsed_metadata = cJSON_GetObjectItem(parsed, "metadataCS");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_metadata, "name");
    const char *name_val = cJSON_GetStringValue(parsed_name);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s;enabled=%d", name_val ? name_val : "null", (int)is_enabled);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}