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
//<ID> 1226
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
    double number_value = 0.0;
    cJSON_bool invalid_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a numeric item and a string reference, add both to the array
    num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(arr, num_item);
    str_ref = cJSON_CreateStringReference("static_str");
    cJSON_AddItemToArray(arr, str_ref);

    // step 4: Operate - retrieve the first array element and read its numeric value and validity
    first_item = cJSON_GetArrayItem(arr, 0);
    number_value = cJSON_GetNumberValue(first_item);
    invalid_flag = cJSON_IsInvalid(first_item);

    // step 5: Validate - derive a small score from meaningful data flow
    validation_score = (int)(number_value > 100.0) + (int)(invalid_flag == 0) + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0);

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}