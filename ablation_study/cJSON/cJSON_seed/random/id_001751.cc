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
//<ID> 1751
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached_arr = (cJSON *)0;
    cJSON *retrieved_answer = (cJSON *)0;
    int nums[4] = {10, 20, 30, 40};
    int arr_size = 0;
    double answer_value = 0.0;
    cJSON_bool is_answer_number = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateIntArray(nums, 4);
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate
    detached_arr = cJSON_DetachItemFromObject(root, "values");
    arr_size = cJSON_GetArraySize(detached_arr);
    retrieved_answer = cJSON_GetObjectItem(root, "answer");
    is_answer_number = cJSON_IsNumber(retrieved_answer);
    answer_value = cJSON_GetNumberValue(retrieved_answer);

    // step 5: Validate
    validation = (int)(arr_size == 4) + (int)(is_answer_number != 0) + (int)(answer_value == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_arr);
    (void)root;
    (void)arr;
    (void)num_item;
    (void)detached_arr;
    (void)retrieved_answer;
    (void)arr_size;
    (void)answer_value;
    (void)is_answer_number;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}