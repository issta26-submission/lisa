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
//<ID> 1544
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddTrueToObject(root, "available");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    const float values_arr[3] = { 1.5f, 2.5f, 3.5f };
    cJSON *values = cJSON_CreateFloatArray(values_arr, 3);
    cJSON_AddItemToObject(data, "values", values);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(data, "pi", pi);

    // step 3: Operate and Validate
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    cJSON *v2 = cJSON_GetArrayItem(values, 2);
    double dv0 = cJSON_GetNumberValue(v0);
    double dv1 = cJSON_GetNumberValue(v1);
    double dv2 = cJSON_GetNumberValue(v2);
    double dpi = cJSON_GetNumberValue(pi);
    double sum = dv0 + dv1 + dv2 + dpi;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    double average = sum / 4.0;
    cJSON *avg_item = cJSON_CreateNumber(average);
    cJSON_AddItemToObject(root, "average", avg_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (int)(sum) % 10);
    scratch[2] = (char)('0' + (int)(average) % 10);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}