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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    cJSON *items = cJSON_AddArrayToObject(data, "items");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);
    cJSON *template_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(template_obj, "type", "prototype");
    cJSON_AddItemToObject(data, "template", template_obj);
    cJSON *dup = cJSON_Duplicate(template_obj, 1);
    cJSON_AddItemToArray(items, dup);

    // step 3: Operate and Validate
    cJSON *retrieved_data = cJSON_GetObjectItem(root, "data");
    cJSON *retrieved_items = cJSON_GetObjectItem(retrieved_data, "items");
    cJSON *first_item = cJSON_GetArrayItem(retrieved_items, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first_item);
    cJSON_AddBoolToObject(root, "first_is_null", first_is_null);
    cJSON *retrieved_template = cJSON_GetObjectItem(retrieved_data, "template");
    const char *type_str = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_template, "type"));
    cJSON_AddStringToObject(root, "template_type_copy", type_str);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}