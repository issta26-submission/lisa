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
//<ID> 1385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *true_in_arr = NULL;
    cJSON *data_arr = NULL;
    cJSON *element0 = NULL;
    cJSON *cmp_ref = NULL;
    int *nums = NULL;
    cJSON_bool is_array = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = (int *)cJSON_malloc(sizeof(int) * 3);
    nums[0] = 7;
    nums[1] = 8;
    nums[2] = 9;
    arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "data", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    true_in_arr = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_in_arr);

    // step 3: Operate / Validate
    data_arr = cJSON_GetObjectItem(root, "data");
    is_array = cJSON_IsArray(data_arr);
    element0 = cJSON_GetArrayItem(data_arr, 0);
    cmp_ref = cJSON_CreateNumber(42.0);
    cmp_result = cJSON_Compare(element0, cmp_ref, 1);
    (void)is_array;
    (void)cmp_result;

    // step 4: Cleanup
    cJSON_Delete(cmp_ref);
    cJSON_free(nums);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}