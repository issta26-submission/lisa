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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw", "{\"flag\":true,\"count\":5}");
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "info", "generated");
    cJSON_AddBoolToObject(meta, "enabled", 1);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(float_array);
    cJSON *second = cJSON_GetArrayItem(float_array, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *answer_item = cJSON_GetObjectItem(root, "answer");
    double answer_val = cJSON_GetNumberValue(answer_item);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    int summary = count + (int)root_is_obj + (int)second_val + (int)answer_val + root->type + float_array->type + raw_item->type + meta->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}