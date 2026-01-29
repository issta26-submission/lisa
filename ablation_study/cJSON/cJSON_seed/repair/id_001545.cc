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
//<ID> 1545
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
    cJSON_AddItemToObject(root, "meta", meta);
    float nums[3] = { 1.5f, 2.5f, 3.0f };
    cJSON *values = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddBoolToObject(root, "visible", (cJSON_bool)0);
    cJSON *base = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "base", base);

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(meta, "threshold", threshold);
    cJSON_AddTrueToObject(meta, "configured");

    // step 3: Operate and Validate
    cJSON *v0_item = cJSON_GetArrayItem(values, 0);
    cJSON *v1_item = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(v0_item);
    double v1 = cJSON_GetNumberValue(v1_item);
    double sum = v0 + v1 + cJSON_GetNumberValue(base);
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum_with_base", sum_item);
    cJSON_AddBoolToObject(meta, "sum_exceeds_threshold", (cJSON_bool)(sum > cJSON_GetNumberValue(threshold)));
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : 0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : 'x';
    scratch[1] = (char)('0' + (int)(cJSON_IsTrue(cJSON_GetObjectItem(root, "active")) ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}