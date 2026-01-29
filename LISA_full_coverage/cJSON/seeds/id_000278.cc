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
//<ID> 278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.1f, 2.2f, 3.3f};
    cJSON *farr = cJSON_CreateFloatArray(nums, 3);
    const char *words[3] = {"alpha", "beta", "gamma"};
    cJSON *sarr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "floats", farr);
    cJSON_AddItemToObject(root, "strings", sarr);
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "note", "configured");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal_case = cJSON_Compare(root, copy, 1);
    cJSON_AddBoolToObject(root, "equal_case", equal_case);
    cJSON_bool floats_is_array = cJSON_IsArray(farr);
    cJSON_bool strings_is_array = cJSON_IsArray(sarr);
    cJSON_AddBoolToObject(root, "floats_is_array", floats_is_array);
    cJSON_AddBoolToObject(root, "strings_is_array", strings_is_array);

    // step 3: Operate and Validate
    int float_count = cJSON_GetArraySize(farr);
    int string_count = cJSON_GetArraySize(sarr);
    cJSON_AddNumberToObject(root, "float_count", (double)float_count);
    cJSON_AddNumberToObject(root, "string_count", (double)string_count);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = float_count + string_count + (int)equal_case;
    (void)summary;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}