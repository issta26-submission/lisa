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
//<ID> 437
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"meta\":{\"name\":\"example\"},\"items\":[\"a\",\"b\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(meta, "name");
    cJSON_bool name_invalid = cJSON_IsInvalid(name_item);
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *ref = cJSON_CreateStringReference("referenced");
    cJSON_AddItemToArray(new_arr, ref);
    cJSON_AddItemToObject(root, "new_array", new_arr);

    // step 3: Operate
    cJSON *new_array_found = cJSON_GetObjectItemCaseSensitive(root, "new_array");
    int arr_size = cJSON_GetArraySize(new_array_found);
    cJSON *first = cJSON_GetArrayItem(new_array_found, 0);
    const char *first_val = cJSON_GetStringValue(first);
    (void)meta_is_obj;
    (void)name_invalid;
    (void)name_str;
    (void)first_val;
    (void)arr_size;

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}