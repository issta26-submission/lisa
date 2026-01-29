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
//<ID> 1221
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
    double number_value = 0.0;
    cJSON_bool invalid_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "payload", arr);

    // step 3: Configure - create a numeric item and a string reference, add both to the array
    num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num_item);
    str_ref = cJSON_CreateStringReference("constant_label");
    cJSON_AddItemToArray(arr, str_ref);

    // step 4: Operate - read number value and check validity of the string reference
    number_value = cJSON_GetNumberValue(num_item);
    invalid_flag = cJSON_IsInvalid(str_ref);
    validation_score = (int)number_value + (int)invalid_flag + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0);

    // step 5: Cleanup - delete root to free whole structure
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}