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
//<ID> 1754
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_array = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    int nums[3] = {1, 2, 3};
    int arr_size = 0;
    cJSON_bool is_detached_number = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_array = cJSON_CreateIntArray(nums, 3);
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate
    detached = cJSON_DetachItemFromObject(root, "answer");
    retrieved_array = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(retrieved_array);
    is_detached_number = cJSON_IsNumber(detached);

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(is_detached_number != 0) + (int)(detached != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);
    (void)root;
    (void)int_array;
    (void)num_item;
    (void)retrieved_array;
    (void)arr_size;
    (void)is_detached_number;
    (void)nums;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}