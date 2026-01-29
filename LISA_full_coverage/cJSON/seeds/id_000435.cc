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
//<ID> 435
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"root\",\"items\":[\"alpha\",\"beta\"],\"count\":2}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);

    // step 2: Configure
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_item);
    cJSON *new_array = cJSON_CreateArray();
    cJSON *ref_to_first = cJSON_CreateStringReference(first_str);
    cJSON *ref_literal = cJSON_CreateStringReference("added_item");
    cJSON_AddItemToArray(new_array, ref_literal);
    cJSON_AddItemToArray(new_array, ref_to_first);
    cJSON_AddItemToObject(root, "new_items", new_array);

    // step 3: Operate
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_items);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)root_is_obj;
    (void)first_is_invalid;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}