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
//<ID> 1373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[3] = {1, 2, 3};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", int_arr);
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *orig_item = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "item", orig_item);

    // step 2: Configure
    cJSON_bool has_item = cJSON_HasObjectItem(root, "item");
    cJSON *has_item_num = cJSON_CreateNumber((double)has_item);
    cJSON_AddItemToObject(root, "has_item", has_item_num);
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_dup", child_dup);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "item", replacement);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}