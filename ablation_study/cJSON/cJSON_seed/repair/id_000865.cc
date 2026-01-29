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
//<ID> 865
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "name", "example_project");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 2: Configure
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddStringToObject(entry, "type", "sample");
    cJSON_AddNumberToObject(entry, "count", 3.0);
    cJSON_AddItemToObject(root, "entry", entry);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(entry, "optional", optional_null);
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "enabled", enabled_true);

    // step 3: Operate and Validate
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    cJSON_AddNumberToObject(root, "version_copy", version_val);
    cJSON *enabled_item = cJSON_GetObjectItem(meta, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_item);
    double enabled_numeric = (double)(is_enabled + 0); // convert to double without branches
    cJSON_AddNumberToObject(root, "enabled_flag", enabled_numeric);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}