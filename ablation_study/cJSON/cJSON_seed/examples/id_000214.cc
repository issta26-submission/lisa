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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {3.14, 2.718, 1.618};
    cJSON *double_array = cJSON_CreateDoubleArray(nums, 3);
    const char *strings_arr[2] = {"alpha", "beta"};
    cJSON *string_array = cJSON_CreateStringArray(strings_arr, 2);
    cJSON *maybe_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "doubles", double_array);
    cJSON_AddItemToObjectCS(root, "strings", string_array);
    cJSON_AddItemToObjectCS(root, "nullable", maybe_null);
    int dbl_count = cJSON_GetArraySize(double_array);
    cJSON *count_num = cJSON_CreateNumber((double)dbl_count);
    cJSON_AddItemToObjectCS(root, "double_count", count_num);
    cJSON_bool null_flag = cJSON_IsNull(maybe_null);
    cJSON *isnull_num = cJSON_CreateNumber((double)null_flag);
    cJSON_AddItemToObjectCS(root, "is_null_flag", isnull_num);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}