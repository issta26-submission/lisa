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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = "{\"nums\":[1,2,3],\"flag\":false}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON_bool has_flag = cJSON_HasObjectItem(root, "flag");

    // step 2: Configure
    int extra_nums[] = {7, 8, 9};
    cJSON *int_arr = cJSON_CreateIntArray(extra_nums, 3);
    cJSON_AddItemToObject(root, "int_array", int_arr);
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddFalseToObject(root, "added_false");
    cJSON_AddNumberToObject(root, "has_flag", (double)has_flag);

    // step 3: Operate and Validate
    cJSON *nums = cJSON_GetObjectItem(root, "nums");
    int nums_size = cJSON_GetArraySize(nums);
    cJSON *first_item = cJSON_GetArrayItem(nums, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_value", first_value);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}