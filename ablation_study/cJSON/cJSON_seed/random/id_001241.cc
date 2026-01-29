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
//<ID> 1241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *parent = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *sub_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    int nums[3];
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    int count = 3;
    int extracted_value = 0;
    cJSON_bool is_arr = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an inner int array, attach array to a parent array
    root = cJSON_CreateObject();
    parent = cJSON_CreateArray();
    int_arr = cJSON_CreateIntArray(nums, count);
    cJSON_AddItemToArray(parent, int_arr);
    cJSON_AddItemToObject(root, "container", parent);

    // step 3: Operate - retrieve the inner array via the parent and verify it's an array, extract second element
    sub_item = cJSON_GetArrayItem(parent, 0);
    is_arr = cJSON_IsArray(sub_item);
    num_item = cJSON_GetArrayItem(sub_item, 1);
    extracted_value = num_item->valueint;

    // step 4: Modify - delete the second element from the inner int array
    cJSON_DeleteItemFromArray(sub_item, 1);

    // step 5: Validate and Cleanup - compute a small validation score and free all resources
    validation_score = (int)is_arr + extracted_value + cJSON_GetArraySize(sub_item);
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}