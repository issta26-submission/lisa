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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    float floats[] = {1.5f, 2.5f, 3.5f};
    cJSON *float_array = cJSON_CreateFloatArray(floats, 3);
    cJSON_AddItemToObject(root, "float_array", float_array);
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *string_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "string_array", string_array);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "initialized");
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal_before = cJSON_Compare(root, copy, 1);
    cJSON_AddStringToObject(copy, "modified", "yes");
    cJSON_bool equal_after = cJSON_Compare(root, copy, 1);

    // step 3: Operate and Validate
    cJSON_bool items_is_array = cJSON_IsArray(arr);
    cJSON_bool floats_is_array = cJSON_IsArray(float_array);
    cJSON_bool strings_is_array = cJSON_IsArray(string_array);
    int items_size = cJSON_GetArraySize(arr);
    int floats_size = cJSON_GetArraySize(float_array);
    int strings_size = cJSON_GetArraySize(string_array);
    cJSON_AddBoolToObject(root, "equal_before", equal_before);
    cJSON_AddBoolToObject(root, "equal_after", equal_after);
    cJSON_AddBoolToObject(root, "items_is_array", items_is_array);
    cJSON_AddBoolToObject(root, "floats_is_array", floats_is_array);
    cJSON_AddBoolToObject(root, "strings_is_array", strings_is_array);
    cJSON_AddNumberToObject(root, "items_size", (double)items_size);
    cJSON_AddNumberToObject(root, "floats_size", (double)floats_size);
    cJSON_AddNumberToObject(root, "strings_size", (double)strings_size);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}