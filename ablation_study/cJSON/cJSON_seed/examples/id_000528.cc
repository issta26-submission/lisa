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
//<ID> 528
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(meta, "enabled", enabled_item);
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *name_item = cJSON_CreateString("demo-name");
    cJSON_AddItemToObjectCS(config, "name", name_item);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed_meta, "enabled");
    int enabled_flag = (int)cJSON_IsTrue(parsed_enabled);
    cJSON *parsed_config = cJSON_GetObjectItem(parsed, "config");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_config, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s;enabled=%d", name_str ? name_str : "null", enabled_flag);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}