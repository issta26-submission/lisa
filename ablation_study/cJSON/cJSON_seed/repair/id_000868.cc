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
//<ID> 868
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddStringToObject(root, "id", "root1");
    cJSON_AddNumberToObject(root, "version", 1.2);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "count", 5.0);
    cJSON_AddStringToObject(child, "note", "child_note");
    cJSON_AddItemToObject(config, "child", child);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional", null_item);

    // step 3: Operate and Validate
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver = cJSON_GetNumberValue(ver_item);
    cJSON_AddNumberToObject(root, "version_copy", ver);
    cJSON *imported_child = cJSON_GetObjectItem(config, "child");
    cJSON *count_item = cJSON_GetObjectItem(imported_child, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "imported_count", count_val);
    cJSON_bool is_enabled = cJSON_IsTrue(cJSON_GetObjectItem(config, "enabled"));
    cJSON_AddNumberToObject(root, "enabled_flag", (double)is_enabled);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}