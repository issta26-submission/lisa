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
//<ID> 1532
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    const char *raw_json = "{\"threshold\":3.14,\"modes\":[\"auto\",\"manual\"]}";
    cJSON_AddRawToObject(config, "metadata", raw_json);

    // step 3: Operate & Validate
    char *printed = cJSON_Print(root);
    cJSON *meta_item = cJSON_GetObjectItemCaseSensitive(config, "metadata");
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(config, "enabled");
    cJSON_bool meta_is_raw = cJSON_IsRaw(meta_item);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    const char *meta_string = cJSON_GetStringValue(meta_item);
    (void)meta_is_raw; (void)enabled_is_bool; (void)meta_string;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}