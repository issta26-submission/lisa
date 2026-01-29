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
//<ID> 1347
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *main_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "main", main_array);

    // step 2: Configure
    int nums[4] = {10, 20, 30, 40};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToArray(main_array, int_arr);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON *sref = cJSON_CreateStringReference("constant_string");
    cJSON_AddItemToObject(root, "name_ref", sref);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(main_array, int_arr);
    cJSON_AddItemToObject(root, "detached_ints", detached);
    int size_after_detach = cJSON_GetArraySize(main_array);
    cJSON *got_detached = cJSON_GetObjectItem(root, "detached_ints");
    cJSON_bool is_arr = cJSON_IsArray(got_detached);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    const char *sval = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name_ref"));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = sval ? sval[0] : 0;
    ((char *)scratch)[2] = (char)('0' + (size_after_detach % 10));
    ((char *)scratch)[3] = (char)('0' + (is_arr ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}