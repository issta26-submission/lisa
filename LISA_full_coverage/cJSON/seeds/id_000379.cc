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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *raw_item = cJSON_CreateRaw("7");
    cJSON_AddItemToArray(numbers, raw_item);

    // step 2: Configure
    int count = cJSON_GetArraySize(numbers);
    cJSON *second_item = cJSON_GetArrayItem(numbers, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);

    // step 3: Operate
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "count", (double)count);
    cJSON_AddNumberToObject(summary, "second_value", second_value);
    cJSON_AddNumberToObject(summary, "raw_is_raw", (double)raw_is_raw);
    cJSON_AddItemToObject(root, "summary", summary);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}