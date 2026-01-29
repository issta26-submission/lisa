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
//<ID> 1533
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
    const char *raw_meta = "{\"version\":1,\"notes\":\"raw data\"}";
    cJSON_AddRawToObject(config, "metadata", raw_meta);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    char *printed_root = cJSON_Print(root);
    cJSON *config_item = cJSON_GetObjectItemCaseSensitive(root, "config");

    // step 3: Operate & Validate
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(config_item, "enabled");
    cJSON *metadata_item = cJSON_GetObjectItemCaseSensitive(config_item, "metadata");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON_bool metadata_is_raw = cJSON_IsRaw(metadata_item);
    const char *metadata_value = cJSON_GetStringValue(metadata_item);
    (void)enabled_is_bool; (void)metadata_is_raw; (void)metadata_value;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}