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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = { 1.5f, 2.5f, -3.0f };
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *number_item = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "float_values", float_array);
    cJSON_AddItemToObject(root, "meta", child_obj);
    cJSON_AddItemToObject(root, "answer", number_item);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_blob", "{\"inner\":7}");

    // step 3: Operate and Validate
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    double answer = cJSON_GetNumberValue(number_item);
    int array_size = cJSON_GetArraySize(float_array);
    cJSON_bool meta_is_object = cJSON_IsObject(child_obj);
    int summary = (int)answer + array_size + (int)meta_is_object + root->type + (raw_item ? raw_item->type : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}