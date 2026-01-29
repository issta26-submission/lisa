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
//<ID> 1371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[4] = {1.0, 2.0, 3.5, 4.25};
    cJSON *nums_arr = cJSON_CreateDoubleArray(nums, 4);
    cJSON *value_item = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON_AddItemToObject(data, "nums", nums_arr);
    cJSON_AddItemToObject(root, "value", value_item);
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 3: Operate
    cJSON *dup_nums = cJSON_Duplicate(nums_arr, 1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "value", dup_nums);
    cJSON_bool has_value = cJSON_HasObjectItem(root, "value");
    cJSON *exists_flag = cJSON_CreateBool(has_value);
    cJSON_AddItemToObject(root, "value_exists", exists_flag);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}