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
//<ID> 276
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
    cJSON_AddItemToObject(root, "floats", farr);
    const char *strs[3] = {"alpha", "beta", "gamma"};
    cJSON *sarr = cJSON_CreateStringArray(strs, 3);
    cJSON_AddItemToObject(root, "strings", sarr);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "greeting", "hello world");
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    cJSON_AddStringToObject(root, "compare_result", equal ? "equal" : "different");
    cJSON_AddStringToObject(root, "floats_is_array", cJSON_IsArray(farr) ? "yes" : "no");
    cJSON_AddStringToObject(root, "strings_is_array", cJSON_IsArray(sarr) ? "yes" : "no");

    // step 3: Operate and Validate
    int fcount = cJSON_GetArraySize(farr);
    int scount = cJSON_GetArraySize(sarr);
    cJSON_AddNumberToObject(root, "floats_count", (double)fcount);
    cJSON_AddNumberToObject(root, "strings_count", (double)scount);
    cJSON *firstf = cJSON_GetArrayItem(farr, 0);
    double first_val = cJSON_GetNumberValue(firstf);
    cJSON_AddNumberToObject(root, "first_float", first_val);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = fcount + scount + (int)equal + (int)cJSON_IsArray(farr) + (int)cJSON_IsArray(sarr) + (int)first_val + (printed ? (int)printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    if (printed) cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(copy);

    // API sequence test completed successfully
    return 66;
}