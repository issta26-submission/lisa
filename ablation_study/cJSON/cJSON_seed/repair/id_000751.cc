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
//<ID> 751
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and compose root structure
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure: add raw payload and explicit null option to config
    cJSON *raw_blob = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "raw_blob", raw_blob);
    cJSON_AddItemToObject(config, "optional", optional_null);
    cJSON_AddStringToObject(root, "version", "1.0");

    // step 3: Operate and Validate: retrieve items and record validations in meta
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *raw_ref = cJSON_GetObjectItem(config_ref, "raw_blob");
    cJSON *optional_ref = cJSON_GetObjectItem(config_ref, "optional");
    cJSON_AddBoolToObject(meta, "raw_is_raw", cJSON_IsRaw(raw_ref));
    cJSON_AddBoolToObject(meta, "optional_is_null", cJSON_IsNull(optional_ref));
    cJSON_AddStringToObject(meta, "raw_preview", "{\"embedded\":true}");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}