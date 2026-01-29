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
//<ID> 1578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    int nums[4] = {10, 20, 30, 40};
    cJSON *arr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "nums", arr);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Operate
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "nums_copy", dup_arr);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *computed = cJSON_CreateNumber(second_value + 5.5);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *dup_third_item = cJSON_GetArrayItem(dup_arr, 2);
    double dup_third_value = cJSON_GetNumberValue(dup_third_item);
    cJSON *dup_check = cJSON_CreateNumber(dup_third_value);
    cJSON_AddItemToObject(root, "dup_third", dup_check);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "nums_copy");
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}