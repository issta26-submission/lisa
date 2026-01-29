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
//<ID> 914
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "arr";
    const char *num_direct_key = "num_direct";
    const char *num_item_key = "num_item";
    const char *str_key = "message";
    const char *str_value = "hello world";
    double number_value = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *num_direct_item = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool invalid_check = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize / Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(number_value);

    // step 3: Configure - attach items using different add APIs (string, number, item, array)
    num_direct_item = cJSON_AddNumberToObject(root, num_direct_key, number_value);
    cJSON_AddStringToObject(root, str_key, str_value);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);
    cJSON_AddItemToObject(root, num_item_key, num_item);

    // step 4: Operate - populate the array with values so it is meaningful
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));

    // step 5: Validate - use cJSON_IsInvalid and aggregate simple checks
    invalid_check = cJSON_IsInvalid(num_direct_item);
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)(num_direct_item != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(invalid_check == 0);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}