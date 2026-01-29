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
//<ID> 279
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.0f, 2.5f, -3.14f};
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    const char *strings[2] = {"alpha", "beta"};
    cJSON *string_arr = cJSON_CreateStringArray(strings, 2);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemToObject(root, "strings", string_arr);
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 2: Configure
    cJSON *mirror = cJSON_CreateObject();
    cJSON *float_arr_dup = cJSON_Duplicate(float_arr, 1);
    cJSON *string_arr_dup = cJSON_Duplicate(string_arr, 1);
    cJSON_AddItemToObject(mirror, "floats_copy", float_arr_dup);
    cJSON_AddItemToObject(mirror, "strings_copy", string_arr_dup);
    cJSON_AddStringToObject(mirror, "status_copy", "initialized_copy");
    cJSON_AddItemToObject(root, "mirror", mirror);

    // step 3: Operate and Validate
    cJSON_bool float_is_array = cJSON_IsArray(float_arr);
    cJSON_bool string_is_array = cJSON_IsArray(string_arr);
    int float_count = cJSON_GetArraySize(float_arr);
    int string_count = cJSON_GetArraySize(string_arr);
    cJSON_AddNumberToObject(root, "float_count", (double)float_count);
    cJSON_AddNumberToObject(root, "string_count", (double)string_count);
    cJSON_bool floats_equal = cJSON_Compare(float_arr, float_arr_dup, 1);
    cJSON_bool strings_equal = cJSON_Compare(string_arr, string_arr_dup, 1);
    cJSON_AddBoolToObject(root, "floats_equal", floats_equal);
    cJSON_AddBoolToObject(root, "strings_equal", strings_equal);
    char *dump = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}