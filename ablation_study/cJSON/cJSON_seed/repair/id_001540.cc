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
//<ID> 1540
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
    float nums[3] = { 1.0f, 2.5f, -3.25f };
    cJSON *farr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", farr);
    cJSON_AddTrueToObject(root, "ready");
    cJSON_AddBoolToObject(root, "active", 0);
    cJSON *base = cJSON_CreateNumber(10.5);
    cJSON_AddItemToObject(meta, "base", base);

    // step 2: Configure
    cJSON_AddBoolToObject(meta, "flag", 1);
    cJSON *inc = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "increment", inc);

    // step 3: Operate and Validate
    double base_val = cJSON_GetNumberValue(base);
    double inc_val = cJSON_GetNumberValue(inc);
    cJSON *arr_item = cJSON_GetArrayItem(farr, 1);
    double arr_val = cJSON_GetNumberValue(arr_item);
    double total = base_val + inc_val + arr_val;
    cJSON *total_num = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(meta, "total", total_num);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out && out[0] ? out[0] : 'X';
    scratch[1] = (char)('0' + (int)(total > 0.0));
    scratch[2] = (char)('#');
    cJSON_free(scratch);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}