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
//<ID> 1229
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
    cJSON *str_ref = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    int array_size = 0;
    double number_value = 0.0;
    cJSON_bool invalid_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array container
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create a number and a string reference, add both into the array, attach array to root
    num_item = cJSON_CreateNumber(3.14);
    str_ref = cJSON_CreateStringReference("string_reference");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - inspect the array and extract the numeric value and invalid-flag of the string reference
    array_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    number_value = cJSON_GetNumberValue(first_item);
    invalid_flag = cJSON_IsInvalid(str_ref);

    // step 5: Validate and Cleanup - compute a small validation score and free all resources
    validation_score = array_size + (int)(number_value == 3.14) + (int)(!invalid_flag) + (int)(str_ref != (cJSON *)0) + (int)(num_item != (cJSON *)0);
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}