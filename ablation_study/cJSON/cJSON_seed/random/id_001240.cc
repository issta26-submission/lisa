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
//<ID> 1240
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
    cJSON *intArray = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *second_value_item = (cJSON *)0;
    cJSON_bool first_is_array = 0;
    double second_value = 0.0;
    int nums[3] = {10, 20, 30};
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, and an int-array from C ints
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    intArray = cJSON_CreateIntArray(nums, 3);

    // step 3: Configure - add the created int-array into the array, add an extra numeric item, and attach array to root
    cJSON_AddItemToArray(arr, intArray);
    num_item = cJSON_CreateNumber(999.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate - fetch the first element (the nested int-array), verify it's an array, extract its second element value, then delete that first array element from parent array
    first_item = cJSON_GetArrayItem(arr, 0);
    first_is_array = cJSON_IsArray(first_item);
    second_value_item = cJSON_GetArrayItem(first_item, 1);
    second_value = cJSON_GetNumberValue(second_value_item);
    cJSON_DeleteItemFromArray(arr, 0);

    // step 5: Validate - compute a small validation score using meaningful outputs from above operations
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(intArray != (cJSON *)0) + (int)(first_item != (cJSON *)0) + (int)first_is_array + (int)(second_value == 20.0) + (int)(num_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which cleans up attached structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}