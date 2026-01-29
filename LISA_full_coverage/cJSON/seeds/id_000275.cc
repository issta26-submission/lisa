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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float floats_arr_vals[3] = {1.0f, 2.5f, 3.75f};
    cJSON *float_array = cJSON_CreateFloatArray(floats_arr_vals, 3);
    const char *strs[3] = {"one", "two", "three"};
    cJSON *string_array = cJSON_CreateStringArray(strs, 3);
    cJSON_AddItemToObject(root, "float_array", float_array);
    cJSON_AddItemToObject(root, "string_array", string_array);
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "creator", "tester");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON_bool floats_are_array = cJSON_IsArray(float_array);
    cJSON_bool strings_are_array = cJSON_IsArray(string_array);
    cJSON_AddBoolToObject(root, "floats_are_array", floats_are_array);
    cJSON_AddBoolToObject(root, "strings_are_array", strings_are_array);
    cJSON *string_array_dup = cJSON_Duplicate(string_array, 1);
    cJSON_bool arrays_equal = cJSON_Compare(string_array, string_array_dup, 1);
    cJSON_AddBoolToObject(root, "strings_equal_dup", arrays_equal);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(string_array_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}