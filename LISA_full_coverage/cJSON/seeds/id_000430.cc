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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"example\",\"items\":[\"alpha\",\"beta\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON_bool root_invalid = cJSON_IsInvalid(root);
    cJSON_bool root_is_obj = cJSON_IsObject(root);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *ref_first = cJSON_CreateStringReference(first_str);
    cJSON_AddItemToArray(new_arr, ref_first);
    cJSON_AddItemToObject(root, "new_items", new_arr);

    // step 3: Operate
    cJSON *ref_extra = cJSON_CreateStringReference("added_via_ref");
    cJSON_AddItemToArray(new_arr, ref_extra);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    int original_size = cJSON_GetArraySize(items);
    cJSON *detached_name = cJSON_DetachItemFromObject(root, "name");
    cJSON_bool detached_is_obj = cJSON_IsObject(detached_name);
    (void)root_invalid;
    (void)root_is_obj;
    (void)original_size;
    (void)detached_is_obj;
    cJSON_Delete(detached_name);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}