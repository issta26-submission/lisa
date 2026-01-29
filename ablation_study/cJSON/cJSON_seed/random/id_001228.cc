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
//<ID> 1228
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
    cJSON_bool is_invalid = 0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a numeric item and a string reference, add both to the array
    num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(arr, num_item);
    str_ref = cJSON_CreateStringReference("example_ref");
    cJSON_AddItemToArray(arr, str_ref);

    // step 4: Operate - retrieve the first array element and obtain its numeric value; check invalid flag on string ref
    first_item = cJSON_GetArrayItem(arr, 0);
    number_value = cJSON_GetNumberValue(first_item);
    is_invalid = cJSON_IsInvalid(str_ref);

    // step 5: Validate - derive a small validation score from meaningful data flow
    array_size = cJSON_GetArraySize(arr);
    validation_score = array_size + (int)(number_value > 100.0) + (int)(!is_invalid);

    // step 6: Cleanup - delete root which frees the array and its items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)number_value;
    (void)is_invalid;
    return 66;
}