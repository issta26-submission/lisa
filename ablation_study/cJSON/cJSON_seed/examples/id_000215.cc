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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3];
    nums[0] = 1.1;
    nums[1] = 2.2;
    nums[2] = 3.3;
    const char *const strs[3] = { "alpha", "beta", "gamma" };
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *str_arr = cJSON_CreateStringArray(strs, 3);

    // step 2: Configure
    cJSON_bool add_dbl_res = cJSON_AddItemToObjectCS(root, "doubles", dbl_arr);
    cJSON_bool add_str_res = cJSON_AddItemToObjectCS(root, "strings", str_arr);
    cJSON *fixed_num = cJSON_CreateNumber(6.6);
    cJSON_bool add_fixed_num_res = cJSON_AddItemToObjectCS(root, "fixed_total", fixed_num);

    // step 3: Operate & Validate
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_bool add_null_res = cJSON_AddItemToObjectCS(root, "maybe", maybe_null);
    cJSON_bool is_null_flag = cJSON_IsNull(maybe_null);
    cJSON *is_null_num = cJSON_CreateNumber((double)is_null_flag);
    cJSON_bool add_flag_res = cJSON_AddItemToObjectCS(root, "is_null_flag", is_null_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}