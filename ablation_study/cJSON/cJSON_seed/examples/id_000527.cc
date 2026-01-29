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
//<ID> 527
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddTrueToObject(meta, "enabled");
    cJSON_AddStringToObject(meta, "name", "demo");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 2: Configure
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "settings", settings);
    cJSON_AddBoolToObject(settings, "featureX", 1);
    cJSON *limits = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "limits", limits);
    cJSON_AddNumberToObject(limits, "max", 10.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_enabled = cJSON_GetObjectItemCaseSensitive(parsed_meta, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(parsed_enabled);
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed_meta, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s;enabled=%d", name_str ? name_str : "null", (int)enabled_flag);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}