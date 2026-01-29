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
//<ID> 1359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.0f, 2.5f, 3.25f};
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON_AddItemToObject(root, "values", float_arr);
    cJSON_AddItemToObject(child, "nullable", null_item);
    cJSON_AddTrueToObject(child, "enabled");

    // step 3: Operate
    cJSON *values = cJSON_GetObjectItem(root, "values");
    int count = cJSON_GetArraySize(values);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "values_count", count_num);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
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