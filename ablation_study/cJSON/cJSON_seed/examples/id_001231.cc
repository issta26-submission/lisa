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
//<ID> 1231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[4] = {1.5, 2.5, 3.5, 4.5};
    cJSON *double_arr = cJSON_CreateDoubleArray(nums, 4);
    cJSON *root = cJSON_CreateObject();
    cJSON *empty_arr = cJSON_CreateArray();
    cJSON *nullable = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", double_arr);
    cJSON_AddItemToObject(root, "list", empty_arr);
    cJSON_AddItemToObject(root, "nullable", nullable);
    cJSON *dup_numbers = cJSON_Duplicate(double_arr, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_numbers);
    cJSON_bool was_null = cJSON_IsNull(nullable);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(double_arr);
    cJSON *third = cJSON_GetArrayItem(double_arr, 2);
    double third_val = cJSON_GetNumberValue(third);
    double computed_val = third_val + (double)size;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}