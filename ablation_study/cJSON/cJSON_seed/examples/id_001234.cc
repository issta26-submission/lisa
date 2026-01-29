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
//<ID> 1234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[] = {1.5, 2.5, 3.5};
    cJSON *double_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "doubles", double_arr);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", null_item);
    cJSON_bool was_null = cJSON_IsNull(null_item);
    cJSON *extra_arr = cJSON_CreateArray();
    cJSON *fixed_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(extra_arr, fixed_num);
    cJSON_AddItemToObject(root, "extra", extra_arr);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(double_arr);
    cJSON *first_elem = cJSON_GetArrayItem(double_arr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}