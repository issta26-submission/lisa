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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums1[3] = {1.1, 2.2, 3.3};
    cJSON *root = cJSON_CreateObject();
    cJSON *double_arr = cJSON_CreateDoubleArray(nums1, 3);
    cJSON *wrapper_array = cJSON_CreateArray();
    cJSON_AddItemToArray(wrapper_array, double_arr);
    cJSON_AddItemToObject(root, "values", wrapper_array);

    // step 2: Configure
    cJSON *false_item = cJSON_AddFalseToObject(root, "enabled");
    const cJSON *flag_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    double flag_num = (double)cJSON_IsTrue(flag_item);
    double nums2[2] = { flag_num, 42.0 };
    cJSON *extra_arr = cJSON_CreateDoubleArray(nums2, 2);
    cJSON_AddItemToArray(wrapper_array, extra_arr);

    // step 3: Operate and Validate
    int sz = cJSON_GetArraySize(wrapper_array);
    cJSON *first_arr = cJSON_GetArrayItem(wrapper_array, 0);
    cJSON *first_num_item = cJSON_GetArrayItem(first_arr, 0);
    double first_val = cJSON_GetNumberValue(first_num_item);
    cJSON_AddNumberToObject(root, "first_value", first_val);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}