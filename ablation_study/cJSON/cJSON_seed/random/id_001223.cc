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
//<ID> 1223
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
    int array_size = 0;
    double number_value = 0.0;
    int invalid_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a string reference and a numeric item, add both to the array
    str_ref = cJSON_CreateStringReference("sample_ref");
    cJSON_AddItemToArray(arr, str_ref);
    num_item = cJSON_CreateNumber(123.5);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate - read numeric value and check validity of the string reference, obtain array size
    number_value = cJSON_GetNumberValue(num_item);
    invalid_flag = (int)cJSON_IsInvalid(str_ref);
    array_size = cJSON_GetArraySize(arr);

    // step 5: Validate - produce a small validation score from meaningful data flow
    validation_score = array_size + (int)(number_value == 123.5) + (int)(!invalid_flag);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}