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
//<ID> 691
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *items = cJSON_AddArrayToObject(config, "items");
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num0);
    cJSON *str0 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, str0);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(meta, "item_count", (double)count);
    cJSON_AddBoolToObject(meta, "has_items", (cJSON_bool)(count > 0));

    // step 3: Operate and Validate
    cJSON_bool cfg_is_obj = cJSON_IsObject(config);
    cJSON_AddBoolToObject(meta, "config_is_object", cfg_is_obj);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}