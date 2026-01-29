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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", version);
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);
    cJSON *name = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_bool replaced_name = cJSON_ReplaceItemInObjectCaseSensitive(root, "name", new_name);
    cJSON *new_flag = cJSON_CreateTrue();
    cJSON_bool replaced_flag = cJSON_ReplaceItemInObjectCaseSensitive(root, "active", new_flag);

    // step 3: Operate and Validate
    cJSON *active_item = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON_bool is_active = cJSON_IsTrue(active_item);
    cJSON *nums = cJSON_GetObjectItem(root, "numbers");
    cJSON_DeleteItemFromArray(nums, 0);
    int size_after = cJSON_GetArraySize(nums);
    cJSON *remaining = cJSON_GetArrayItem(nums, 0);
    double remaining_val = cJSON_GetNumberValue(remaining);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    int summary = (int)replaced_name + (int)replaced_flag + (int)is_active + size_after + (int)remaining_val;
    (void)summary;
    (void)name_val;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}