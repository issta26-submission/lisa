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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObjectCS(root, "meaning", num_item);
    double darr_vals[] = {1.5, 2.5, 3.5};
    cJSON *darr = cJSON_CreateDoubleArray(darr_vals, 3);
    cJSON_AddItemToObjectCS(root, "doubles", darr);
    const char *strs[] = {"alpha", "beta", "gamma"};
    cJSON *sarr = cJSON_CreateStringArray(strs, 3);
    cJSON_AddItemToObjectCS(root, "strings", sarr);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "nullable", maybe_null);

    // step 2: Configure
    cJSON *null_flag_num = cJSON_CreateNumber((double)cJSON_IsNull(maybe_null));
    cJSON_AddItemToObjectCS(root, "is_null_flag", null_flag_num);
    int dcount = cJSON_GetArraySize(darr);
    cJSON *dcount_num = cJSON_CreateNumber((double)dcount);
    cJSON_AddItemToObjectCS(root, "double_count", dcount_num);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON *equal_flag = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObjectCS(dup, "was_equal", equal_flag);
    char *out_dup = cJSON_PrintUnformatted(dup);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(out_dup);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}