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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double nums[3] = {1.1, 2.2, 3.3};
    cJSON *values_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values_arr);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "ok");
    cJSON *nested_arr = cJSON_CreateArray();
    cJSON *dup_values = cJSON_Duplicate(values_arr, 1);
    cJSON_AddItemToArray(nested_arr, dup_values);
    cJSON_AddItemToObject(root, "nested", nested_arr);

    // step 3: Operate
    cJSON_AddItemToArray(values_arr, cJSON_CreateNumber(4.4));
    cJSON *ok_item = cJSON_GetObjectItem(root, "ok");
    cJSON_bool ok_true = cJSON_IsTrue(ok_item);
    cJSON_AddNumberToObject(root, "ok_flag", (double)ok_true);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}