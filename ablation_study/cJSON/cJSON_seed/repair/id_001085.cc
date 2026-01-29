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
//<ID> 1085
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddFalseToObject(root, "active");
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "measures", float_arr);

    // step 2: Configure
    cJSON *got_arr = cJSON_GetObjectItem(root, "measures");
    int size_before = cJSON_GetArraySize(got_arr);
    cJSON_DeleteItemFromArray(got_arr, 1);

    // step 3: Operate and Validate
    cJSON *detached_false = cJSON_DetachItemFromObject(root, "active");
    cJSON_AddItemToArray(got_arr, detached_false);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (size_before & 0xF));
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}