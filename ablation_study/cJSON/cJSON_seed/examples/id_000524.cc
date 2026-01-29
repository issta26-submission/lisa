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
//<ID> 524
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
    cJSON_AddItemToObjectCS(root, "settings", settings);
    cJSON *enabled_flag = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "enabled", enabled_flag);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_CreateString("demo-name");
    cJSON_AddItemToObjectCS(meta, "name", name_item);

    // step 2: Configure
    cJSON_AddStringToObject(settings, "mode", "fast");
    cJSON_AddNumberToObject(settings, "threshold", 3.14);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(settings, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateString("one"));
    cJSON_AddItemToArray(list, cJSON_CreateString("two"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_meta, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(parsed_enabled);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s;enabled=%d", name_str ? name_str : "null", (int)is_enabled);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}