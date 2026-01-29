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
//<ID> 1087
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float nums[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateFloatArray(nums, 4);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *flag = cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *extra_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(values, extra_num);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON_DeleteItemFromArray(got_values, 1);
    cJSON *detached_values = cJSON_DetachItemFromObject(root, "values");
    int remaining = cJSON_GetArraySize(detached_values);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (remaining & 0xF));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(detached_values);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}