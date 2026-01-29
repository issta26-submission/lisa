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
//<ID> 909
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "arr";
    const char *flag_key = "flag";
    const char *str0 = "alpha";
    double num1 = 7.5;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item_str = (cJSON *)0;
    cJSON *item_num = (cJSON *)0;
    cJSON *bool_false_item = (cJSON *)0;
    cJSON *retrieved_num_item = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool added_flag = (cJSON_bool)0;
    cJSON_bool is_false = (cJSON_bool)0;
    double extracted_num = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_str = cJSON_CreateString(str0);
    item_num = cJSON_CreateNumber(num1);
    bool_false_item = cJSON_CreateFalse();

    // step 3: Configure - build array and attach to root, attach false flag via AddItemToObject
    cJSON_AddItemToArray(arr, item_str);
    cJSON_AddItemToArray(arr, item_num);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);
    added_flag = cJSON_AddItemToObject(root, flag_key, bool_false_item);

    // step 4: Operate - retrieve array element and check boolean
    retrieved_num_item = cJSON_GetArrayItem(arr, 1);
    extracted_num = cJSON_GetNumberValue(retrieved_num_item);
    is_false = cJSON_IsFalse(bool_false_item);

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)added_flag + (int)(retrieved_num_item != (cJSON *)0) + (int)(extracted_num == num1) + (int)is_false;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}