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
//<ID> 431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"alpha\",\"items\":[\"one\",\"two\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *ref_name = cJSON_CreateStringReference(name_str);
    cJSON_AddItemToArray(new_arr, ref_name);
    cJSON *ref_literal = cJSON_CreateStringReference("additional");
    cJSON_AddItemToArray(new_arr, ref_literal);
    cJSON_AddItemToObject(root, "added", new_arr);

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first_item);
    cJSON_AddItemReferenceToArray(new_arr, first_item);
    int orig_size = cJSON_GetArraySize(items);
    int new_size = cJSON_GetArraySize(new_arr);
    const char *first_text = cJSON_GetStringValue(first_item);
    (void)root_is_object;
    (void)first_invalid;
    (void)orig_size;
    (void)new_size;
    (void)first_text;

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}