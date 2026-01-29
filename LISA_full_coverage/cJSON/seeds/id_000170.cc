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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.1f, 2.2f, 3.3f};
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "float_values", float_array);
    const char *raw_json = "{\"nested\":{\"value\":42}}";
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_payload", raw_json);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 7.0);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "enabled");

    // step 3: Operate and Validate
    cJSON_bool meta_is_object = cJSON_IsObject(meta);
    double count_value = cJSON_GetNumberValue(count_item);
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    int float_count = cJSON_GetArraySize(float_array);
    cJSON *first_float_item = cJSON_GetArrayItem(float_array, 0);
    double first_float_value = cJSON_GetNumberValue(first_float_item);
    int summary = (int)count_value + float_count + (int)meta_is_object + (int)first_float_value + root->type + raw_item->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}