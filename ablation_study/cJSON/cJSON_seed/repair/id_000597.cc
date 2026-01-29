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
//<ID> 597
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums_arr[4];
    nums_arr[0] = 10;
    nums_arr[1] = 20;
    nums_arr[2] = 30;
    nums_arr[3] = 40;
    cJSON *numbers = cJSON_CreateIntArray(nums_arr, 4);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *version_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(meta, "version", version_item);

    // step 2: Configure
    int count = cJSON_GetArraySize(numbers);
    cJSON_AddBoolToObject(root, "has_many", (cJSON_bool)(count > 3));
    cJSON *tag_item = cJSON_CreateString("dataset");
    cJSON_AddItemToObject(root, "tag", tag_item);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 2));
    double v3 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 3));
    double sum = v0 + v1 + v2 + v3;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    double average = sum / (double)count;
    cJSON *avg_item = cJSON_CreateNumber(average);
    cJSON_AddItemToObject(root, "average", avg_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}