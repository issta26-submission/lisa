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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const double doubles_arr[3] = {1.1, 2.2, 3.3};
    const char *const strings_arr[3] = {"alpha", "beta", "gamma"};
    cJSON *root = cJSON_CreateObject();
    cJSON *doubles = cJSON_CreateDoubleArray(doubles_arr, 3);
    cJSON *strings = cJSON_CreateStringArray(strings_arr, 3);
    cJSON *nullable = cJSON_CreateNull();

    // step 2: Configure
    cJSON_bool added_d = cJSON_AddItemToObjectCS(root, "doubles", doubles);
    cJSON_bool added_s = cJSON_AddItemToObjectCS(root, "strings", strings);
    cJSON_bool added_n = cJSON_AddItemToObjectCS(root, "nullable", nullable);

    // step 3: Operate & Validate
    cJSON_bool is_null_flag = cJSON_IsNull(nullable);
    cJSON *isnull_num = cJSON_CreateNumber((double)is_null_flag);
    cJSON_bool added_flag = cJSON_AddItemToObjectCS(root, "is_null", isnull_num);
    double added_sum = (double)added_d + (double)added_s + (double)added_n + (double)added_flag;
    cJSON *added_sum_num = cJSON_CreateNumber(added_sum);
    cJSON_bool added_sum_flag = cJSON_AddItemToObjectCS(root, "add_count", added_sum_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}