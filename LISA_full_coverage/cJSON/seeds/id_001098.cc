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
//<ID> 1098
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3],\"maybe_null\":null,\"val\":4.5,\"name\":\"old\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *arr_item = cJSON_GetObjectItem(root, "arr");
    cJSON *maybe_null_item = cJSON_GetObjectItem(root, "maybe_null");
    cJSON *val_item = cJSON_GetObjectItem(root, "val");
    cJSON_bool arr_check = cJSON_IsArray(arr_item);
    cJSON_bool null_check = cJSON_IsNull(maybe_null_item);
    double val_double = cJSON_GetNumberValue(val_item);
    cJSON *replacement_name = cJSON_CreateString("new_name");
    cJSON *new_number = cJSON_CreateNumber(val_double * 2.0);
    cJSON_bool rep1 = cJSON_ReplaceItemViaPointer(root, name_item, replacement_name);
    cJSON_bool rep2 = cJSON_ReplaceItemViaPointer(root, val_item, new_number);
    cJSON_AddNumberToObject(root, "replaced_count", (double)(rep1 + rep2));
    cJSON_AddBoolToObject(root, "arr_is_array", arr_check);
    cJSON_AddBoolToObject(root, "was_null", null_check);

    // step 3: Operate
    double confirmed_val = cJSON_GetNumberValue(new_number);
    char *out = cJSON_Print(root);

    // step 4: Validate & Cleanup
    (void)confirmed_val;
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}