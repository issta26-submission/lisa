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
//<ID> 1341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[4] = {10, 20, 30, 40};
    cJSON *numbers = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", extras);
    cJSON *label_ref = cJSON_CreateStringReference("example_ref");
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);

    // step 2: Configure
    cJSON *elem1 = cJSON_CreateStringReference("elem_one");
    cJSON_AddItemToArray(extras, elem1);
    cJSON *elem2 = cJSON_CreateStringReference("elem_two");
    cJSON_AddItemToArray(extras, elem2);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(numbers);
    cJSON *detached_flag = cJSON_DetachItemViaPointer(root, flag);
    cJSON_bool was_false = cJSON_IsFalse(detached_flag);
    int extras_count = cJSON_GetArraySize(extras);
    double second_value = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 1));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    const char *label_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (size_before % 10));
    ((char *)scratch)[2] = (char)('0' + (extras_count % 10));
    ((char *)scratch)[3] = (char)('0' + ((int)second_value % 10));
    ((char *)scratch)[4] = label_str ? label_str[0] : 0;
    ((char *)scratch)[5] = (char)('0' + (was_false ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(detached_flag);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}