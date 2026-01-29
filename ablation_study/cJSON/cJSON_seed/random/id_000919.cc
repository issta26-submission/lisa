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
//<ID> 919
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *str_key = "name";
    const char *num_key = "value";
    const char *name_val = "example";
    double num_val = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_num0 = (cJSON *)0;
    cJSON *arr_num1 = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool is_invalid_str = (cJSON_bool)0;
    cJSON_bool is_invalid_num = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    arr_num0 = cJSON_CreateNumber(1.0);
    arr_num1 = cJSON_CreateNumber(2.0);

    // step 3: Configure - populate array and attach to root
    cJSON_AddItemToArray(arr, arr_num0);
    cJSON_AddItemToArray(arr, arr_num1);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);

    // step 4: Core operations - add string and number to root using specified helpers
    str_item = cJSON_AddStringToObject(root, str_key, name_val);
    num_item = cJSON_AddNumberToObject(root, num_key, num_val);

    // step 5: Operate / Validate - check invalid status and compute simple score
    is_invalid_str = cJSON_IsInvalid(str_item);
    is_invalid_num = cJSON_IsInvalid(num_item);
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)(str_item != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(is_invalid_str == 0) + (int)(is_invalid_num == 0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}