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
//<ID> 1541
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
    cJSON_AddBoolToObject(root, "flagged", 0);
    cJSON *base_num = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "base_value", base_num);
    float vals[3] = { 1.0f, 2.5f, -3.75f };
    cJSON *float_arr = cJSON_CreateFloatArray(vals, 3);
    cJSON_AddItemToObject(root, "float_array", float_arr);

    // step 2: Configure
    cJSON *retrieved = cJSON_GetObjectItem(root, "base_value");
    double base_val = cJSON_GetNumberValue(retrieved);
    double computed_val = base_val * 2.0 + 1.0;
    cJSON *computed_num = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_num);
    cJSON_AddBoolToObject(root, "computed_is_whole", (cJSON_bool)(computed_val == (double)(int)computed_val));

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : 'x';
    scratch[1] = (char)('0' + ((int)base_val % 10));
    scratch[2] = (char)('0' + ((int)computed_val % 10));
    scratch[3] = cJSON_IsArray(float_arr) ? 'A' : 'a';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}