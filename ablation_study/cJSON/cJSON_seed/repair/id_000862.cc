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
//<ID> 862
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
    cJSON *id_item = cJSON_AddStringToObject(root, "id", "root1");
    cJSON *version_item = cJSON_AddNumberToObject(root, "version", 3.14);
    cJSON *enabled_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled_flag);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "maybe", maybe_null);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "count", 5.0);
    cJSON_AddStringToObject(child, "note", "child_note");
    cJSON_AddItemToObject(config, "child", child);
    cJSON *ref_string = cJSON_AddStringToObject(config, "mode", "active");

    // step 3: Operate and Validate
    cJSON *cfg_lookup = cJSON_GetObjectItem(root, "config");
    cJSON *flag_lookup = cJSON_GetObjectItem(cfg_lookup, "enabled");
    double enabled_numeric = (double)cJSON_IsTrue(flag_lookup);
    cJSON_AddNumberToObject(root, "enabled_numeric", enabled_numeric);
    cJSON *ver_lookup = cJSON_GetObjectItem(root, "version");
    double ver_value = cJSON_GetNumberValue(ver_lookup);
    cJSON_AddNumberToObject(root, "version_copy", ver_value);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}