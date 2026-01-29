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
//<ID> 1756
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
    cJSON *detached_item = (cJSON *)0;
    int numbers[3];
    int arr_size = 0;
    cJSON_bool detached_is_number = 0;
    int validation = 0;

    // step 2: Initialize
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(numbers, 3);
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", int_arr);
    cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate
    detached_item = cJSON_DetachItemFromObject(root, "answer");
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "numbers"));
    detached_is_number = cJSON_IsNumber(detached_item);

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(detached_is_number != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_item);
    (void)root;
    (void)int_arr;
    (void)num_item;
    (void)detached_item;
    (void)numbers;
    (void)arr_size;
    (void)detached_is_number;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}