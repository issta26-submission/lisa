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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const float nums[] = {1.0f, 2.5f, -3.14f};
    cJSON *farr = cJSON_CreateFloatArray(nums, 3);
    const char *strs[] = {"alpha", "beta", "gamma"};
    cJSON *sarr = cJSON_CreateStringArray(strs, 3);
    cJSON_AddItemToObject(root, "floats", farr);
    cJSON_AddItemToObject(root, "strings", sarr);
    cJSON *g = cJSON_AddStringToObject(root, "greeting", "hello");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *sref = cJSON_CreateStringReference("const_ref");
    cJSON_AddItemToObject(root, "sref", sref);

    // step 3: Operate and Validate
    cJSON_bool f_is_array = cJSON_IsArray(farr);
    cJSON_bool s_is_array = cJSON_IsArray(sarr);
    cJSON_AddBoolToObject(root, "floats_is_array", f_is_array);
    cJSON_AddBoolToObject(root, "strings_is_array", s_is_array);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup_root, 1);
    cJSON_AddBoolToObject(root, "duplicate_equal", equal);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}