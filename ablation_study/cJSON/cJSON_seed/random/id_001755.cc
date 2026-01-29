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
//<ID> 1755
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
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    const int numbers[] = {1, 2, 3, 4};
    int arr_size = 0;
    double num_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(numbers, 4);
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", int_arr);
    cJSON_AddItemToObject(root, "answer", num_item);
    detached = cJSON_DetachItemFromObject(root, "numbers");
    cJSON_AddItemToObject(root, "detached_numbers", detached);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "detached_numbers");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_num = cJSON_GetObjectItem(root, "answer");
    num_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    validation = (int)(arr_size == 4) + (int)(num_value == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)int_arr;
    (void)num_item;
    (void)detached;
    (void)retrieved_arr;
    (void)retrieved_num;
    (void)arr_size;
    (void)num_value;
    (void)numbers;
    // API sequence test completed successfully
    return 66;
}