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
//<ID> 1089
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.25f, 2.5f, 3.75f};
    cJSON *values = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *flag = cJSON_AddFalseToObject(root, "enabled");

    // step 2: Configure
    cJSON *extra_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, extra_num);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    int size_before = cJSON_GetArraySize(got_values);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *out = cJSON_PrintUnformatted(root);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + (size_before & 0xF));
    cJSON_free(out);
    cJSON_DeleteItemFromArray(values, 1);
    cJSON *detached_flag = cJSON_DetachItemFromObject(root, "enabled");

    // step 4: Cleanup
    cJSON_Delete(detached_flag);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}