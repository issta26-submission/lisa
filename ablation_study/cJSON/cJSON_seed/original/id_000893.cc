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
//<ID> 893
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double dnums[3] = {3.14, 2.71, 1.41};
    float fnums[3] = {1.0f, 2.0f, 3.0f};
    cJSON *double_arr = cJSON_CreateDoubleArray(dnums, 3);
    cJSON *float_arr = cJSON_CreateFloatArray(fnums, 3);
    cJSON_AddItemToObject(root, "doubles", double_arr);
    cJSON_AddItemToObject(root, "floats", float_arr);

    // step 2: Configure
    cJSON *first_double_item = cJSON_GetArrayItem(double_arr, 0);
    cJSON *second_double_item = cJSON_GetArrayItem(double_arr, 1);
    double first_val = cJSON_GetNumberValue(first_double_item);
    double second_val = cJSON_GetNumberValue(second_double_item);
    double pair_sum = first_val + second_val;
    cJSON_AddNumberToObject(root, "sum_first_two", pair_sum);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "floats");

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}