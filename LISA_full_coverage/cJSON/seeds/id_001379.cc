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
//<ID> 1379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[4] = {1, 2, 3, 4};
    cJSON *numbers = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddStringToObject(root, "name", "original_name");

    // step 2: Configure
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON_AddNumberToObject(nested, "inner_value", 123.0);
    cJSON *numbers_dup = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_dup", numbers_dup);
    cJSON *replacement_name = cJSON_CreateString("replaced_name");

    // step 3: Operate
    cJSON_bool has_nested = cJSON_HasObjectItem(root, "nested");
    cJSON *has_nested_item = cJSON_CreateBool(has_nested);
    cJSON_AddItemToObject(root, "has_nested", has_nested_item);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "name", replacement_name);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}