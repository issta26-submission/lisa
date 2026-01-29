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
//<ID> 1224
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
    cJSON *str_ref = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    double number_value = 0.0;
    cJSON_bool invalid_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, attach the array to the root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a string reference and a numeric item and add both into the array
    str_ref = cJSON_CreateStringReference("static_text");
    cJSON_AddItemToArray(arr, str_ref);
    num_item = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate - retrieve the numeric element from the array and obtain its numeric value and invalid status
    retrieved = cJSON_GetArrayItem(arr, 1);
    number_value = cJSON_GetNumberValue(retrieved);
    invalid_flag = cJSON_IsInvalid(retrieved);

    // step 5: Validate - compute a small validation score using meaningful data flow
    validation_score = (int)(number_value == 7.5) + (int)(str_ref != (cJSON *)0) + (int)(!invalid_flag);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)number_value;
    (void)validation_score;
    return 66;
}