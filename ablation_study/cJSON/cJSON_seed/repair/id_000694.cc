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
//<ID> 694
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToArray(items, obj);
    cJSON *name = cJSON_CreateString("child_item");
    cJSON_AddItemToObject(obj, "name", name);

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddBoolToObject(obj, "enabled", 1);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);
    cJSON *meta = cJSON_CreateString("metadata");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON_bool obj_is_obj = cJSON_IsObject(obj);
    cJSON_AddBoolToObject(root, "obj_is_object", obj_is_obj);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)count);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_obj = cJSON_IsObject(first);
    cJSON_AddBoolToObject(root, "first_is_object", first_is_obj);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}