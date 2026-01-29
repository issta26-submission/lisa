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
//<ID> 1091
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3],\"name\":\"orig\",\"val\":3.14,\"maybe\":null}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *val_item = cJSON_GetObjectItem(root, "val");
    double val = cJSON_GetNumberValue(val_item);
    cJSON *new_num = cJSON_CreateNumber(val * 1.5);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON_bool replaced_arr_elem = cJSON_ReplaceItemViaPointer(arr, first_elem, new_num);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_name = cJSON_CreateString("replaced_name");
    cJSON_bool replaced_name = cJSON_ReplaceItemViaPointer(root, name_item, new_name);

    // step 3: Operate
    cJSON_bool arr_check = cJSON_IsArray(arr);
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe_item);
    cJSON *first_after = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_after);
    cJSON_AddNumberToObject(root, "first_val_copy", first_val);
    cJSON_AddNumberToObject(root, "replaced_arr_flag", replaced_arr_elem);
    cJSON_AddNumberToObject(root, "replaced_name_flag", replaced_name);
    cJSON_AddBoolToObject(root, "maybe_was_null", maybe_is_null);
    cJSON_AddNumberToObject(root, "arr_is_array", arr_check);
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}