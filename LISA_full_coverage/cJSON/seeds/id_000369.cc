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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[4] = {1.0, 2.0, 3.5, 4.25};
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    int initial_count = cJSON_GetArraySize(values);
    cJSON *second_item = cJSON_GetArrayItem(values, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *new_num = cJSON_CreateNumber(99.99);
    cJSON_AddItemToArray(values, new_num);

    // step 3: Operate and Validate
    int new_count = cJSON_GetArraySize(values);
    cJSON *last_item = cJSON_GetArrayItem(values, new_count - 1);
    double last_value = cJSON_GetNumberValue(last_item);
    cJSON_AddNumberToObject(root, "initial_count", (double)initial_count);
    cJSON_AddNumberToObject(root, "second_value", second_value);
    cJSON_AddNumberToObject(root, "last_value", last_value);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}