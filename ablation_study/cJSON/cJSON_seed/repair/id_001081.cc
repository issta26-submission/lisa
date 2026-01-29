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
//<ID> 1081
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag_false = cJSON_AddFalseToObject(root, "active");
    float nums[3];
    nums[0] = 1.5f;
    nums[1] = 2.5f;
    nums[2] = 3.5f;
    cJSON *farray = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "metrics", farray);

    // step 2: Configure
    cJSON *metrics_ref = cJSON_GetObjectItem(root, "metrics");
    cJSON_DeleteItemFromArray(metrics_ref, 1);

    // step 3: Operate and Validate
    cJSON *detached_flag = cJSON_DetachItemFromObject(root, "active");
    int remaining = cJSON_GetArraySize(metrics_ref);
    void *buffer = cJSON_malloc(8);
    memset(buffer, 0, 8);
    ((char *)buffer)[0] = (char)('0' + (remaining & 0xF));
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached_flag);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}