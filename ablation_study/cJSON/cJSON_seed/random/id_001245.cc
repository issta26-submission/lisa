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
//<ID> 1245
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
    cJSON *fetched_item = (cJSON *)0;
    int nums[3];
    int count = 3;
    cJSON_bool is_array = 0;
    double fetched_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - prepare integer data and create an int array cJSON node
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    arr = cJSON_CreateIntArray(nums, count);

    // step 3: Configure - create a root object, attach the int array, and append a new number to the array
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "ints", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate - verify the node is an array and fetch the recently added item by index
    is_array = cJSON_IsArray(arr);
    fetched_item = cJSON_GetArrayItem(arr, 3);
    fetched_value = cJSON_GetNumberValue(fetched_item);

    // step 5: Validate - compute a small validation score (no branching)
    validation_score = (int)(is_array) + (int)(fetched_item != (cJSON *)0) + (int)(fetched_value == 42.0);
    (void)validation_score;

    // step 6: Cleanup - remove one element from the array and delete the entire root to free resources
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}