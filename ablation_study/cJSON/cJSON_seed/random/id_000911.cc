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
//<ID> 911
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *root_str_key = "greeting";
    const char *arr_key = "values";
    const char *num_key = "constant";
    const char *greeting = "hello";
    double constant_value = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *created_number = (cJSON *)0;
    cJSON *created_string = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool arr_invalid = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - attach heterogeneous items to root and build the array
    created_number = cJSON_AddNumberToObject(root, num_key, constant_value);
    created_string = cJSON_AddStringToObject(root, root_str_key, greeting);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.5));
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);

    // step 4: Operate - check for invalid state of the array (expected to be not invalid)
    arr_invalid = cJSON_IsInvalid(arr);

    // step 5: Validate - simple aggregated checks
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(created_number != (cJSON *)0) + (int)(created_string != (cJSON *)0) + (int)added_arr + (int)!arr_invalid;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}