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
//<ID> 862
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[4];
    nums[0] = 1.5f;
    nums[1] = 2.5f;
    nums[2] = -3.0f;
    nums[3] = 4.25f;
    cJSON *float_array = cJSON_CreateFloatArray(nums, 4);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON_AddFalseToObject(root, "enabled");
    const char *json_text = " { \"name\": \"example\", \"count\": 42 } ";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(parsed, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name", name_copy);
    cJSON *count_item = cJSON_GetObjectItem(parsed, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *count_copy = cJSON_CreateNumber(count_val);
    cJSON_AddItemToObject(root, "count", count_copy);

    // step 3: Operate
    int array_size = cJSON_GetArraySize(float_array);
    cJSON *a0 = cJSON_GetArrayItem(float_array, 0);
    cJSON *a1 = cJSON_GetArrayItem(float_array, 1);
    cJSON *a2 = cJSON_GetArrayItem(float_array, 2);
    cJSON *a3 = cJSON_GetArrayItem(float_array, 3);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double v3 = cJSON_GetNumberValue(a3);
    double total = v0 + v1 + v2 + v3 + count_val;
    (void)array_size;
    (void)total;

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}