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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *owner_str = cJSON_CreateString("alice");
    cJSON_AddItemToObject(metadata, "owner", owner_str);
    cJSON *null_field = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "misc", null_field);
    cJSON *num_item = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(items, num_item);

    // step 3: Operate & Validate
    cJSON_bool meta_is_obj = cJSON_IsObject(metadata);
    cJSON_AddBoolToObject(root, "metadata_is_object", meta_is_obj);
    cJSON_bool has_owner = cJSON_HasObjectItem(metadata, "owner");
    cJSON_AddBoolToObject(metadata, "has_owner", has_owner);
    cJSON *owner_node = cJSON_GetObjectItem(metadata, "owner");
    const char *owner_val = cJSON_GetStringValue(owner_node);
    cJSON_AddStringToObject(root, "owner_copy", owner_val);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}