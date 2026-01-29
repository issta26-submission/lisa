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
//<ID> 1536
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
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_config", "{\"threshold\":3,\"mode\":\"auto\"}");

    // step 2: Configure
    char *printed_root = cJSON_Print(root);
    cJSON *got_config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *got_raw = cJSON_GetObjectItemCaseSensitive(root, "raw_config");
    cJSON *got_enabled = cJSON_GetObjectItemCaseSensitive(got_config, "enabled");

    // step 3: Operate & Validate
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON_bool raw_is_raw = cJSON_IsRaw(got_raw);
    (void)enabled_is_bool;
    (void)raw_is_raw;
    (void)raw_item;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}