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
//<ID> 1345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);
    cJSON *arr = cJSON_CreateArray();
    cJSON *f = cJSON_CreateFalse();
    cJSON *sref = cJSON_CreateStringReference("hello");
    cJSON_AddItemToArray(arr, f);
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "misc", arr);

    // step 2: Configure
    cJSON *root_name = cJSON_CreateStringReference("root_ref");
    cJSON_AddItemToObject(root, "rootname", root_name);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *detached_false = cJSON_DetachItemViaPointer(arr, first_item);
    int size_after = cJSON_GetArraySize(arr);
    double first_int_value = cJSON_GetNumberValue(cJSON_GetArrayItem(int_array, 0));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    const char *remaining_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    cJSON_bool was_false = cJSON_IsFalse(detached_false);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (size_before % 10));
    ((char *)scratch)[2] = (char)('0' + (size_after % 10));
    ((char *)scratch)[3] = (char)('0' + ((int)first_int_value % 10));
    ((char *)scratch)[4] = remaining_str ? remaining_str[0] : 0;
    ((char *)scratch)[5] = (char)('0' + (was_false ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(detached_false);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}