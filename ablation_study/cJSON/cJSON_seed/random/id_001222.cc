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
//<ID> 1222
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
    cJSON *first_elem = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    double number_value = 0.0;
    cJSON_bool is_invalid = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a string reference and a numeric item, add both to the array
    str_ref = cJSON_CreateStringReference("fuzz_string_ref");
    cJSON_AddItemToArray(arr, str_ref);
    num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate - retrieve elements and read numeric value, check invalid status
    first_elem = cJSON_GetArrayItem(arr, 0);
    second_elem = cJSON_GetArrayItem(arr, 1);
    number_value = cJSON_GetNumberValue(second_elem);
    is_invalid = cJSON_IsInvalid(first_elem);

    // step 5: Validate - compute a small validation score from meaningful data flow
    array_size = cJSON_GetArraySize(arr);
    validation_score = array_size + (int)(number_value == 3.14) + (int)(!is_invalid) + (int)(str_ref != (cJSON *)0);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}