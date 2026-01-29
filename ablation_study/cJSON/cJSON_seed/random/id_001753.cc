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
//<ID> 1753
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
    cJSON *num = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    int numbers_arr[3] = {7, 13, 21};
    int array_size = 0;
    double answer_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_array = cJSON_CreateIntArray(numbers_arr, 3);
    num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddItemToObject(root, "answer", num);

    // step 4: Operate
    detached = cJSON_DetachItemFromObject(root, "numbers");
    cJSON_AddItemToObject(root, "detached_numbers", detached);
    retrieved_num = cJSON_GetObjectItem(root, "answer");
    answer_value = cJSON_GetNumberValue(retrieved_num);
    array_size = cJSON_GetArraySize(detached);

    // step 5: Validate
    validation = (int)(answer_value == 42.0) + (int)(array_size == 3);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)int_array;
    (void)num;
    (void)detached;
    (void)retrieved_num;
    (void)numbers_arr;
    (void)array_size;
    (void)answer_value;
    // API sequence test completed successfully
    return 66;
}