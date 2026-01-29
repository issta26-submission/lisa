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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.5f, -2.0f, 3.1415f};
    cJSON *farr = cJSON_CreateFloatArray(nums, 3);
    const char *words[2] = {"alpha", "beta"};
    cJSON *sarr = cJSON_CreateStringArray(words, 2);
    cJSON_AddItemToObject(root, "float_array", farr);
    cJSON_AddItemToObject(root, "string_array", sarr);
    cJSON *title = cJSON_AddStringToObject(root, "title", "example");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *copy_root = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    cJSON_bool farr_is_array = cJSON_IsArray(farr);
    cJSON_bool sarr_is_array = cJSON_IsArray(sarr);
    int fsize = cJSON_GetArraySize(farr);
    int ssize = cJSON_GetArraySize(sarr);
    cJSON_bool equal = cJSON_Compare(root, copy_root, 1);
    cJSON_AddNumberToObject(root, "float_count", (double)fsize);
    cJSON_AddNumberToObject(root, "string_count", (double)ssize);
    cJSON_AddBoolToObject(root, "arrays_are_arrays", (cJSON_bool)(farr_is_array && sarr_is_array));
    cJSON_AddBoolToObject(root, "root_equals_copy", equal);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(copy_root);

    // API sequence test completed successfully
    return 66;
}