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
//<ID> 1758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *nums = (cJSON *)0;
    cJSON *value = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool is_value_number = 0;
    int validation = 0;
    int ints[4] = {1, 2, 3, 4};

    // step 2: Initialize
    root = cJSON_CreateObject();
    nums = cJSON_CreateIntArray(ints, 4);
    value = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", nums);
    cJSON_AddItemToObject(root, "value", value);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(retrieved);
    detached = cJSON_DetachItemFromObject(root, "numbers");
    cJSON_AddItemToObject(root, "detached", detached);
    retrieved = cJSON_GetObjectItem(root, "detached");
    arr_size = cJSON_GetArraySize(retrieved);
    is_value_number = cJSON_IsNumber(cJSON_GetObjectItem(root, "value"));

    // step 5: Validate
    validation = (int)(arr_size == 4) + (int)(is_value_number != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)nums;
    (void)value;
    (void)retrieved;
    (void)detached;
    (void)arr_size;
    (void)is_value_number;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}