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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float floats_vals[3] = {1.1f, 2.2f, 3.3f};
    cJSON *float_array = cJSON_CreateFloatArray(floats_vals, 3);
    const char *string_vals[3] = {"alpha", "beta", "gamma"};
    cJSON *string_array = cJSON_CreateStringArray(string_vals, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON_AddItemToObject(root, "strings", string_array);
    cJSON_AddStringToObject(root, "greeting", "hello");

    // step 2: Configure
    cJSON *other = cJSON_CreateObject();
    cJSON *float_array2 = cJSON_CreateFloatArray(floats_vals, 3);
    cJSON *string_array2 = cJSON_CreateStringArray(string_vals, 3);
    cJSON_AddItemToObject(other, "floats", float_array2);
    cJSON_AddItemToObject(other, "strings", string_array2);
    cJSON_AddStringToObject(other, "greeting", "hello");

    // step 3: Operate and Validate
    cJSON_bool is_farray = cJSON_IsArray(float_array);
    cJSON_bool is_sarray = cJSON_IsArray(string_array);
    int float_count = cJSON_GetArraySize(float_array);
    cJSON *first_float_item = cJSON_GetArrayItem(float_array, 0);
    double first_float = cJSON_GetNumberValue(first_float_item);
    cJSON_bool are_equal = cJSON_Compare(root, other, 1);
    cJSON_AddNumberToObject(root, "first_float", first_float);
    cJSON_AddNumberToObject(root, "float_count", (double)float_count);
    cJSON_AddBoolToObject(root, "both_are_arrays", (cJSON_bool)(is_farray && is_sarray));
    cJSON_AddBoolToObject(other, "equal_to_root", are_equal);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(other);

    // API sequence test completed successfully
    return 66;
}