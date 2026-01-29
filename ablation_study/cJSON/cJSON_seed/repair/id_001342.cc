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
//<ID> 1342
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
    int nums1[4] = {10, 20, 30, 40};
    cJSON *int_arr1 = cJSON_CreateIntArray(nums1, 4);
    cJSON_AddItemToArray(arr, int_arr1);
    cJSON *str_ref = cJSON_CreateStringReference("example_ref");
    cJSON_AddItemToArray(arr, str_ref);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    int nums2[3] = {7, 8, 9};
    cJSON *int_arr2 = cJSON_CreateIntArray(nums2, 3);
    cJSON *extra = cJSON_CreateArray();
    cJSON_AddItemToArray(extra, int_arr2);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(arr);
    cJSON *detached_int = cJSON_DetachItemViaPointer(arr, int_arr1);
    int rep_nums[2] = {99, 100};
    cJSON *replacement = cJSON_CreateIntArray(rep_nums, 2);
    cJSON_AddItemToArray(arr, replacement);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double v0 = cJSON_GetNumberValue(first);
    const char *maybe_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = maybe_str ? maybe_str[0] : 0;
    ((char *)scratch)[2] = (char)('0' + (size_before % 10));
    ((char *)scratch)[3] = (char)('0' + ((int)v0 % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(detached_int);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}