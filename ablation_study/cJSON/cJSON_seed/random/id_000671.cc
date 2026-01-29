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
//<ID> 671
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num0 = (cJSON *)0;
    cJSON *retrieved_str1 = (cJSON *)0;
    cJSON_bool added_num0 = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_str0 = 0;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_obj = 0;
    double value_num0 = 0.0;
    char *value_str1 = (char *)0;
    int validation_sum = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(42.0);
    num1 = cJSON_CreateNumber(7.5);
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("omega");

    // step 3: Configure
    added_num0 = cJSON_AddItemToArray(arr, num0);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_str0 = cJSON_AddItemToArray(arr, str0);
    added_str1 = cJSON_AddItemToArray(arr, str1);
    added_obj = cJSON_AddItemToObject(root, "mixed", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "mixed");
    retrieved_num0 = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_str1 = cJSON_GetArrayItem(retrieved_arr, 3);
    value_num0 = cJSON_GetNumberValue(retrieved_num0);
    value_str1 = cJSON_GetStringValue(retrieved_str1);

    // step 5: Validate
    validation_sum = (int)(value_num0 > 0.0) + (value_str1 != (char *)0);
    (void)added_num0;
    (void)added_num1;
    (void)added_str0;
    (void)added_str1;
    (void)added_obj;
    (void)retrieved_arr;
    (void)retrieved_num0;
    (void)retrieved_str1;
    (void)value_num0;
    (void)value_str1;
    (void)validation_sum;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}