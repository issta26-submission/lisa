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
//<ID> 1757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached_ints = (cJSON *)0;
    cJSON *detached_number = (cJSON *)0;
    int nums[3];
    int arr_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool is_num = 0;
    int validation = 0;

    // step 2: Initialize
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(nums, 3);
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "ints", int_arr);
    add_ok2 = cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate
    detached_ints = cJSON_DetachItemFromObject(root, "ints");
    arr_size = cJSON_GetArraySize(detached_ints);
    detached_number = cJSON_DetachItemFromObject(root, "answer");
    is_num = cJSON_IsNumber(detached_number);

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(is_num != 0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_ints);
    cJSON_Delete(detached_number);
    (void)root;
    (void)int_arr;
    (void)num_item;
    (void)detached_ints;
    (void)detached_number;
    (void)nums;
    (void)arr_size;
    (void)add_ok1;
    (void)add_ok2;
    (void)is_num;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}