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
//<ID> 146
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *name_str = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name_str);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "optional", maybe_null);
    cJSON *obj_item = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("item1");
    cJSON_AddItemToObject(obj_item, "label", label);
    cJSON_AddItemToArray(items, obj_item);
    cJSON *count_num = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 3: Operate & Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_object = cJSON_IsObject(retrieved_meta);
    const char *retrieved_name = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_meta, "name"));
    cJSON_AddStringToObject(root, "retrieved_name", retrieved_name);
    int item_count = cJSON_GetArraySize(items);
    (void)item_count;
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}