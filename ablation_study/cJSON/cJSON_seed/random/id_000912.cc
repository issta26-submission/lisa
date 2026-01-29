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
//<ID> 912
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "arr";
    const char *str_key = "greeting";
    const char *num_key = "answer";
    const char *scalar_str = "hello_root";
    double number_value = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *num_in_root = (cJSON *)0;
    cJSON *str_in_root = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool is_invalid = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello_array");
    num_item = cJSON_CreateNumber(number_value);

    // step 3: Configure - build array and attach items to root using different add APIs
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);
    str_in_root = cJSON_AddStringToObject(root, str_key, scalar_str);
    num_in_root = cJSON_AddNumberToObject(root, num_key, number_value);

    // step 4: Operate - retrieve the scalar number and check for invalidity
    retrieved_num = cJSON_GetObjectItem(root, num_key);
    is_invalid = cJSON_IsInvalid(retrieved_num);

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)(retrieved_num != (cJSON *)0) + (int)(str_in_root != (cJSON *)0) + (int)(num_in_root != (cJSON *)0) + (int)(!is_invalid);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}