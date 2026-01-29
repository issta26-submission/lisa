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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.5, 2.5, 3.5};
    const char *strs[3] = {"alpha", "beta", "gamma"};

    // step 2: Configure
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObjectCS(root, "double_values", dbl_arr);
    cJSON *str_arr = cJSON_CreateStringArray(strs, 3);
    cJSON_AddItemToObjectCS(root, "string_values", str_arr);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "nullable", maybe_null);

    // step 3: Operate & Validate
    cJSON_bool is_null_flag = cJSON_IsNull(maybe_null);
    cJSON *flag_num = cJSON_CreateNumber((double)is_null_flag);
    cJSON_AddItemToObjectCS(root, "null_flag", flag_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}