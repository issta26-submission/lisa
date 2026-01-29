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
//<ID> 1088
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddFalseToObject(root, "enabled");
    float nums[3] = { 1.1f, 2.2f, 3.3f };
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", float_array);

    // step 2: Configure
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON *extra_num = cJSON_CreateNumber(4.4);
    cJSON_AddItemToArray(got_values, extra_num);
    cJSON_DeleteItemFromArray(got_values, 0);
    cJSON *detached_false = cJSON_DetachItemFromObject(root, "enabled");

    // step 3: Operate and Validate
    int size_after = cJSON_GetArraySize(got_values);
    char *printed = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (size_after & 0xF));
    ((char *)scratch)[1] = printed[0];
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached_false);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}