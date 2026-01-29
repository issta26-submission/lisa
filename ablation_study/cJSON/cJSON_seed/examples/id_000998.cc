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
//<ID> 998
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
    cJSON_AddItemToObjectCS(root, "numbers", numbers);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "info", info);
    cJSON *name = cJSON_CreateString("device-alpha");
    cJSON_AddItemToObjectCS(info, "name", name);
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(info, "tags", tags);
    cJSON *tag0 = cJSON_CreateString("tagA");
    cJSON_AddItemToArray(tags, tag0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *meta_label = cJSON_CreateString("meta-info");
    cJSON_AddItemToObjectCS(meta, "label", meta_label);
    cJSON *copy_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObjectCS(root, "copy_flag", copy_flag);

    // step 3: Operate & Validate
    cJSON_bool has_nums = cJSON_HasObjectItem(root, "numbers");
    cJSON *has_item = cJSON_CreateBool(has_nums);
    cJSON_AddItemToObjectCS(root, "has_numbers", has_item);
    cJSON *nums_from_root = cJSON_GetObjectItem(root, "numbers");
    int count = cJSON_GetArraySize(nums_from_root);
    cJSON *count_item = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObjectCS(root, "numbers_count", count_item);
    cJSON *name_item = cJSON_GetObjectItem(info, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObjectCS(root, "name_copy", name_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}