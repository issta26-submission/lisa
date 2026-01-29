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
//<ID> 679
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON *version = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(cfg, "version", version);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", flag);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(cfg, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON_AddBoolToObject(meta, "name_is_string", name_is_string);
    cJSON *cfg_copy = cJSON_Duplicate(cfg, 1);
    cJSON_AddItemToObject(root, "config_copy", cfg_copy);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}