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
//<ID> 522
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
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(settings, "enabled", enabled_item);
    cJSON *options = cJSON_AddObjectToObject(settings, "options");
    cJSON *mode_item = cJSON_CreateString("high");
    cJSON_AddItemToObjectCS(options, "mode", mode_item);
    cJSON *version_item = cJSON_CreateString("1.2.3");
    cJSON_AddItemToObjectCS(root, "version", version_item);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "threshold", 42.0);
    cJSON *note_ref = cJSON_CreateStringReference("static-note");
    cJSON_AddItemToObjectCS(root, "note", note_ref);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_version = cJSON_GetObjectItem(parsed, "version");
    const char *ver = cJSON_GetStringValue(parsed_version);
    cJSON *parsed_settings = cJSON_GetObjectItem(parsed, "settings");
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed_settings, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(parsed_enabled);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;enabled=%d", ver ? ver : "null", (int)enabled_true);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}