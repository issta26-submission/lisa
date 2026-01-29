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
//<ID> 1546
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
    cJSON *active_item = cJSON_AddTrueToObject(root, "active");
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *pi_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(meta, "pi", pi_num);
    cJSON *count_num = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "count", count_num);
    float values_arr[3] = { 1.0f, 2.0f, 4.0f };
    cJSON *values = cJSON_CreateFloatArray(values_arr, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    cJSON *v2 = cJSON_GetArrayItem(values, 2);
    double sum = cJSON_GetNumberValue(v0) + cJSON_GetNumberValue(v1) + cJSON_GetNumberValue(v2) + cJSON_GetNumberValue(pi_num);
    cJSON *sum_num = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_num);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (int)cJSON_IsTrue(enabled_item));
    scratch[2] = (char)('0' + (int)cJSON_IsTrue(active_item));
    scratch[3] = (char)('0' + (int)cJSON_IsBool(pi_num));
    scratch[4] = '#';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}