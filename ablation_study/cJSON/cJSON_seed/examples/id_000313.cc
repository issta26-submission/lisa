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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *outer = cJSON_CreateArray();
    int nums[3] = {10, 20, 30};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);
    cJSON *num_pi = cJSON_CreateNumber(3.1415);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(outer, int_arr);
    cJSON_AddItemToArray(outer, num_pi);
    cJSON_AddItemToArray(outer, false_item);
    cJSON_AddItemToObject(root, "mixed", outer);

    // step 3: Operate & Validate
    cJSON *got_num = cJSON_GetArrayItem(outer, 1);
    double got_value = cJSON_GetNumberValue(got_num);
    cJSON *replica = cJSON_CreateNumber(got_value);
    cJSON_AddItemToArray(int_arr, replica);
    cJSON *inner_item = cJSON_GetArrayItem(int_arr, 3);
    double inner_value = cJSON_GetNumberValue(inner_item);
    cJSON *final_num = cJSON_CreateNumber(inner_value + 1.0);
    cJSON_AddItemToArray(outer, final_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}