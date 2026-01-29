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
//<ID> 1227
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
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    double extracted_number = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a string reference and a numeric item and add both to the array
    str_ref = cJSON_CreateStringReference("referenced_string");
    cJSON_AddItemToArray(arr, str_ref);
    num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate - retrieve array elements and extract numeric value and invalid status
    first_item = cJSON_GetArrayItem(arr, 0);
    second_item = cJSON_GetArrayItem(arr, 1);
    extracted_number = cJSON_GetNumberValue(second_item);
    validation_score = (int)(!cJSON_IsInvalid(first_item));

    // step 5: Validate - compute a small validation score from meaningful data flow
    validation_score += (int)(extracted_number == 123.0) + (int)(str_ref != (cJSON *)0) + (int)(num_item != (cJSON *)0);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}