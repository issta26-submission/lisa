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
//<ID> 1752
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
    cJSON *detached_arr = (cJSON *)0;
    cJSON *retrieved_answer = (cJSON *)0;
    int nums[4];
    int arr_count = 0;
    int detached_size = 0;
    cJSON_bool answer_is_number = 0;
    int validation = 0;

    // step 2: Initialize
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    nums[3] = 40;
    arr_count = 4;
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(nums, arr_count);
    num_item = cJSON_CreateNumber(123.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate
    detached_arr = cJSON_DetachItemFromObject(root, "ints");
    retrieved_answer = cJSON_GetObjectItemCaseSensitive(root, "answer");

    // step 5: Validate
    detached_size = cJSON_GetArraySize(detached_arr);
    answer_is_number = cJSON_IsNumber(retrieved_answer);
    validation = (int)(detached_size == arr_count) + (int)(answer_is_number != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached_arr);
    cJSON_Delete(root);
    (void)root;
    (void)int_arr;
    (void)num_item;
    (void)detached_arr;
    (void)retrieved_answer;
    (void)nums;
    (void)arr_count;
    (void)detached_size;
    (void)answer_is_number;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}