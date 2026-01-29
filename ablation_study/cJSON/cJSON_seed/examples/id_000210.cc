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
//<ID> 210
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
    const char *strs[3] = {"one", "two", "three"};
    cJSON *darr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *sarr = cJSON_CreateStringArray(strs, 3);
    cJSON *nullable = cJSON_CreateNull();
    cJSON *initial_number = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "double_array", darr);
    cJSON_AddItemToObjectCS(root, "string_array", sarr);
    cJSON_AddItemToObjectCS(root, "nullable", nullable);
    cJSON_AddItemToObjectCS(root, "initial_number", initial_number);

    // step 3: Operate & Validate
    cJSON_bool null_flag = cJSON_IsNull(nullable);
    cJSON *null_flag_num = cJSON_CreateNumber((double)null_flag);
    cJSON_AddItemToObjectCS(root, "null_flag_numeric", null_flag_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}