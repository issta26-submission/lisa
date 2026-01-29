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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float nums[4] = {1.5f, 2.5f, -3.0f, 4.25f};
    cJSON *root = cJSON_CreateObject();
    cJSON *float_array = cJSON_CreateFloatArray(nums, 4);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "name", "example");

    // step 2: Configure (serialize)
    char *pretty = cJSON_Print(root);
    char *buffered = cJSON_PrintBuffered(root, 128, 1);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "floats");
    int count = cJSON_GetArraySize(retrieved);
    cJSON *first_item = cJSON_GetArrayItem(retrieved, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    int summary = count + (int)first_val + (int)meta_is_obj;
    int probe = summary + (pretty ? (int)pretty[0] : 0) + (buffered ? (int)buffered[0] : 0);
    (void)probe;

    // step 4: Cleanup
    cJSON_free(pretty);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}