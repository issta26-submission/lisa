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
//<ID> 1082
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *values = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *flag_false = cJSON_AddFalseToObject(root, "disabled");
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddStringToObject(nested, "name", "inner");
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    cJSON *temp = cJSON_CreateString("temp");
    cJSON_AddItemToArray(values, temp);
    cJSON_DeleteItemFromArray(values, 1);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON *got_nested = cJSON_GetObjectItem(root, "nested");
    cJSON *detached = cJSON_DetachItemFromObject(root, "nested");
    int count = cJSON_GetArraySize(got_values);
    char *printed = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = printed[0];
    ((char *)scratch)[1] = (char)('0' + (count & 0xF));
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}