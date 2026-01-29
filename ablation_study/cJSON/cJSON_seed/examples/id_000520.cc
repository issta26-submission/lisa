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
//<ID> 520
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
    cJSON *name_item = cJSON_CreateString("demo-service");
    cJSON_AddItemToObjectCS(config, "name", name_item);
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(config, "enabled", enabled_item);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta_cs", meta);
    cJSON_AddStringToObject(meta, "author", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_config = cJSON_GetObjectItemCaseSensitive(parsed, "config");
    cJSON *parsed_enabled = cJSON_GetObjectItemCaseSensitive(parsed_config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(parsed_enabled);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_config, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "name=%s;enabled=%d", name_str ? name_str : "null", (int)is_enabled);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}