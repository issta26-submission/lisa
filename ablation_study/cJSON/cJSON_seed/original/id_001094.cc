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
//<ID> 1094
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1.25,2.5],\"val\":10,\"maybe\":null,\"name\":\"original\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *replacement_name = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(root, name_item, replacement_name);
    cJSON *val_item = cJSON_GetObjectItem(root, "val");
    double val_num = cJSON_GetNumberValue(val_item);
    cJSON *val_plus = cJSON_CreateNumber(val_num + 1.5);
    cJSON_AddItemToObject(root, "val_plus", val_plus);

    // step 3: Operate
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON_bool arr_check = cJSON_IsArray(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *dup_number = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToArray(arr, dup_number);
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool is_null = cJSON_IsNull(maybe_item);

    // step 4: Validate & Cleanup
    char *out = cJSON_Print(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}