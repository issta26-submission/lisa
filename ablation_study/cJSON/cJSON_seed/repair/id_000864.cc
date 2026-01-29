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
//<ID> 864
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
    cJSON_AddNumberToObject(root, "version", 3.14);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", true_item);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional", null_item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "count", 7.0);
    cJSON_AddStringToObject(child, "note", "child_note");
    cJSON_AddItemToObject(config, "child", child);

    // step 3: Operate and Validate
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver = cJSON_GetNumberValue(ver_item);
    cJSON_AddNumberToObject(root, "version_copy", ver);
    cJSON_bool enabled_flag = cJSON_IsTrue(cJSON_GetObjectItem(config, "enabled"));
    cJSON_AddNumberToObject(root, "enabled_flag", (double)enabled_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}