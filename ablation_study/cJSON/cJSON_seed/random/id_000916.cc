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
//<ID> 916
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *num_key = "primary";
    const char *str_key = "label";
    const char *str_value = "example";
    double primary_value = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *added_num_ptr = (cJSON *)0;
    cJSON *added_str_ptr = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_arr_item = (cJSON *)0;
    cJSON_bool is_invalid_num = (cJSON_bool)0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize / Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    arr_item0 = cJSON_CreateNumber(10.0);
    arr_item1 = cJSON_CreateNumber(20.0);

    // step 3: Configure - build array and attach to root, add number and string to root
    cJSON_AddItemToArray(arr, arr_item0);
    cJSON_AddItemToArray(arr, arr_item1);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);
    added_num_ptr = cJSON_AddNumberToObject(root, num_key, primary_value);
    added_str_ptr = cJSON_AddStringToObject(root, str_key, str_value);

    // step 4: Operate - retrieve items and perform checks using cJSON_IsInvalid
    retrieved_num = cJSON_GetObjectItem(root, num_key);
    retrieved_arr = cJSON_GetObjectItem(root, arr_key);
    retrieved_arr_item = cJSON_GetArrayItem(retrieved_arr, 1);
    is_invalid_num = cJSON_IsInvalid(retrieved_num);
    extracted_value = cJSON_GetNumberValue(retrieved_arr_item);

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)(added_num_ptr != (cJSON *)0) + (int)(added_str_ptr != (cJSON *)0) + (int)(retrieved_num != (cJSON *)0) + (int)(retrieved_arr_item != (cJSON *)0) + (int)(extracted_value == 20.0) + (int)(!is_invalid_num);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}