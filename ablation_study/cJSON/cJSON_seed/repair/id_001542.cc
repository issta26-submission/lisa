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
//<ID> 1542
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "is_active");
    float floats[3] = {3.14f, -1.5f, 0.25f};
    cJSON *farray = cJSON_CreateFloatArray(floats, 3);
    cJSON_AddItemToObject(root, "float_values", farray);
    cJSON_AddBoolToObject(root, "is_valid", 1);
    cJSON *base_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "base", base_num);

    // step 2: Configure
    cJSON *offset = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "offset", offset);
    cJSON *combined = cJSON_CreateNumber(cJSON_GetNumberValue(base_num) + cJSON_GetNumberValue(offset));
    cJSON_AddItemToObject(root, "combined", combined);

    // step 3: Operate and Validate
    cJSON *got_base = cJSON_GetObjectItemCaseSensitive(root, "base");
    cJSON *got_array = cJSON_GetObjectItemCaseSensitive(root, "float_values");
    double base_val = cJSON_GetNumberValue(got_base);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(got_array, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(got_array, 1));
    double sum = base_val + v0 + v1;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)(sum != 0.0));
    scratch[1] = out_len ? out[0] : '#';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}