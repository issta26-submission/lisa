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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddItemToObject(root, "meta", meta);
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", float_array);

    // step 2: Configure
    char *pretty = cJSON_Print(root);
    char *buffered = cJSON_PrintBuffered(root, 128, 1);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "floats");
    int size = cJSON_GetArraySize(retrieved);
    cJSON *first = cJSON_GetArrayItem(retrieved, 0);
    cJSON *second = cJSON_GetArrayItem(retrieved, 1);
    cJSON *third = cJSON_GetArrayItem(retrieved, 2);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    double v3 = cJSON_GetNumberValue(third);
    double sum = v1 + v2 + v3;
    cJSON_bool meta_is_obj = cJSON_IsObject(cJSON_GetObjectItem(root, "meta"));
    int summary = (int)size + (int)sum + (int)meta_is_obj;
    (void)summary;
    (void)pretty;
    (void)buffered;

    // step 4: Cleanup
    cJSON_free(pretty);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}