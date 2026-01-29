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
//<ID> 1480
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    char *str0 = (char *)0;
    int arr_size = 0;
    double sum = 0.0;
    int validation_score = 0;
    cJSON_bool is_raw1 = 0;
    const float nums[3] = {1.5f, 2.5f, 3.25f};

    // step 2: Initialize - create a float array and a root object
    arr = cJSON_CreateFloatArray(nums, 3);
    root = cJSON_CreateObject();

    // step 3: Configure - attach array to root
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate - inspect the array and its elements
    arr_size = cJSON_GetArraySize(arr);
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    elem2 = cJSON_GetArrayItem(arr, 2);
    sum = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1) + cJSON_GetNumberValue(elem2);
    str0 = cJSON_GetStringValue(elem0);
    is_raw1 = cJSON_IsRaw(elem1);

    // step 5: Validate - derive a small validation score from API results
    validation_score = arr_size + (int)is_raw1 + (int)(str0 != (char *)0) + (int)(sum > 0.0);

    // step 6: Cleanup - delete the JSON tree and silence unused-variable warnings
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum;
    (void)elem0;
    (void)elem1;
    (void)elem2;
    (void)str0;
    (void)arr_size;
    (void)nums;
    // API sequence test completed successfully
    return 66;
}