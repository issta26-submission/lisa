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
//<ID> 1366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nums\":[1.5,2.5,3.75],\"value\":100}";
    size_t json_len = strlen(json);
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_GetObjectItem(parsed, "nums");
    float sample_floats[4] = {7.1f, 8.2f, 9.3f, 10.4f};
    cJSON *float_arr = cJSON_CreateFloatArray(sample_floats, 4);
    cJSON *nums_ref = cJSON_CreateArrayReference(nums);
    cJSON_AddItemToObject(root, "original_nums", nums_ref);
    cJSON_AddItemToObject(root, "float_arr", float_arr);
    cJSON *first_item = cJSON_GetArrayItem(nums, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_item);
    cJSON *flag_num = cJSON_CreateNumber((double)first_is_number);
    cJSON_AddItemToObject(root, "first_is_num", flag_num);

    // step 3: Operate
    int nums_count = cJSON_GetArraySize(nums);
    cJSON *count_num = cJSON_CreateNumber((double)nums_count);
    cJSON_AddItemToObject(root, "nums_count", count_num);
    cJSON *float_dup = cJSON_Duplicate(float_arr, 1);
    cJSON_AddItemToObject(root, "float_arr_dup", float_dup);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}