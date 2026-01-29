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
//<ID> 1080
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.0f, 2.5f, 3.75f};
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_array);

    // step 2: Configure
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(list, num1);
    cJSON_AddItemToArray(list, num2);
    cJSON *flag_false = cJSON_AddFalseToObject(root, "ready");

    // step 3: Operate and Validate
    cJSON *got_floats = cJSON_GetObjectItem(root, "floats");
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    int size_before = cJSON_GetArraySize(got_list);
    cJSON_DeleteItemFromArray(list, 1);
    int size_after = cJSON_GetArraySize(got_list);
    double first_float_value = cJSON_GetNumberValue(cJSON_GetArrayItem(float_array, 0));
    cJSON *detached = cJSON_DetachItemFromObject(root, "floats");
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (size_before & 0xF));
    ((char *)scratch)[1] = (char)('0' + (size_after & 0xF));
    ((char *)scratch)[2] = (char)('0' + (((int)first_float_value) & 0xF));
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}