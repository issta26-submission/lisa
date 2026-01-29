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
//<ID> 690
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n2);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "enabled", flag_false);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *cfg_lookup = cJSON_GetObjectItem(root, "config");
    cJSON_bool cfg_is_obj = cJSON_IsObject(cfg_lookup);
    cJSON_AddBoolToObject(meta, "config_is_object", cfg_is_obj);
    cJSON_AddBoolToObject(meta, "has_items", (count > 0));
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}