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
//<ID> 1349
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
    int nums[4] = {10, 20, 30, 40};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToArray(arr, int_arr);
    cJSON *str_ref = cJSON_CreateStringReference("example_ref");
    cJSON_AddItemToArray(arr, str_ref);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "type", "test_sequence");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(arr);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, second_item);
    int size_after = cJSON_GetArraySize(arr);
    cJSON_AddItemToObject(root, "detached", detached);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(80);
    memset(scratch, 0, 80);
    const char *detached_str = cJSON_GetStringValue(detached);
    cJSON_bool was_false = cJSON_IsFalse(false_item);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = detached_str ? detached_str[0] : 0;
    ((char *)scratch)[2] = (char)('0' + (size_before % 10));
    ((char *)scratch)[3] = (char)('0' + (size_after % 10));
    ((char *)scratch)[4] = (char)('0' + (was_false ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}