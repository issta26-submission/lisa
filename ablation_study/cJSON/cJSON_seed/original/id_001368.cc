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
//<ID> 1368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nums\":[1,2.5,3],\"value\":42} trailing";
    size_t json_len = strlen(json);
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    float extra_vals[4] = {7.5f, 8.25f, 9.0f, -1.0f};
    cJSON *float_arr = cJSON_CreateFloatArray(extra_vals, 4);
    cJSON_AddItemToObject(root, "extras", float_arr);
    cJSON *nums = cJSON_GetObjectItem(parsed, "nums");
    cJSON *nums_ref = cJSON_CreateArrayReference(nums);
    cJSON_AddItemToObject(root, "nums_ref", nums_ref);

    // step 3: Operate
    cJSON *val = cJSON_GetObjectItem(parsed, "value");
    cJSON_bool val_is_number = cJSON_IsNumber(val);
    cJSON *isnum_num = cJSON_CreateNumber((double)val_is_number);
    cJSON_AddItemToObject(root, "value_is_number", isnum_num);
    cJSON *dup_float = cJSON_Duplicate(float_arr, 1);
    cJSON_AddItemToObject(root, "dup_extras", dup_float);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}