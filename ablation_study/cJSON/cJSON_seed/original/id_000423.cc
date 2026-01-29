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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *initial_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "value", initial_num);
    int nums[3] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(int_array);
    cJSON *replacement_num = cJSON_CreateNumber((double)arr_size * 3.14);

    // step 3: Operate
    cJSON_ReplaceItemInObject(root, "value", replacement_num);
    cJSON *current_value = cJSON_GetObjectItem(root, "value");
    double current_val = cJSON_GetNumberValue(current_value);
    cJSON *validated_num = cJSON_CreateNumber(current_val + 1.0);
    cJSON_AddItemToObject(root, "validated", validated_num);
    char *snapshot = cJSON_PrintBuffered(root, 128, 1);
    cJSON_free(snapshot);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}