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
//<ID> 1548
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
    cJSON_AddBoolToObject(root, "enabled", 0);
    const float nums[3] = { 1.0f, 2.5f, -3.25f };
    cJSON *float_array = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", float_array);
    cJSON *threshold = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "threshold", threshold);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "owner", "fuzz_tester");
    cJSON_AddNumberToObject(meta, "scale", 2.0);

    // step 3: Operate and Validate
    cJSON *vals = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *first_val_item = cJSON_GetArrayItem(vals, 0);
    double first_val = cJSON_GetNumberValue(first_val_item);
    cJSON *th_item = cJSON_GetObjectItemCaseSensitive(root, "threshold");
    double th_val = cJSON_GetNumberValue(th_item);
    double computed = first_val + th_val;
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "sum", computed_num);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('A' + ((int)computed % 26));
    scratch[1] = out_len ? out[0] : 'X';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}