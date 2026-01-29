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
//<ID> 1249
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
    cJSON *extra = (cJSON *)0;
    cJSON *third_item = (cJSON *)0;
    int numbers[4] = {10, 20, 30, 40};
    cJSON_bool is_arr = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and an integer array from C integers
    root = cJSON_CreateObject();
    arr = cJSON_CreateIntArray(numbers, 4);

    // step 3: Configure - create an extra numeric item, append it to the array, and attach the array to the root
    extra = cJSON_CreateNumber(99);
    cJSON_AddItemToArray(arr, extra);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate - retrieve an element, check array-ness and measure size, then delete an element
    third_item = cJSON_GetArrayItem(arr, 2);
    is_arr = cJSON_IsArray(arr);
    size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 1);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score to ensure data flowed through APIs
    validation_score = (int)(third_item != (cJSON *)0) + (int)is_arr + size_before + size_after + (int)(extra != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root (which also frees the attached array and remaining items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}