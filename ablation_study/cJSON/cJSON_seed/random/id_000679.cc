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
//<ID> 679
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
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *ret_num_item = (cJSON *)0;
    cJSON *ret_str_item = (cJSON *)0;
    double num_value = 0.0;
    char *str_value = (char *)0;
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_arr_to_obj = 0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(3.14159);
    str = cJSON_CreateString("pi");

    // step 3: Configure
    added_num = cJSON_AddItemToArray(arr, num);
    added_str = cJSON_AddItemToArray(arr, str);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    ret_num_item = cJSON_GetArrayItem(retrieved_arr, 0);
    ret_str_item = cJSON_GetArrayItem(retrieved_arr, 1);
    num_value = cJSON_GetNumberValue(ret_num_item);
    str_value = cJSON_GetStringValue(ret_str_item);
    arr_size = cJSON_GetArraySize(retrieved_arr);

    // step 5: Validate
    (void)added_num;
    (void)added_str;
    (void)added_arr_to_obj;
    (void)num_value;
    (void)str_value;
    (void)arr_size;
    (void)ret_num_item;
    (void)ret_str_item;
    (void)retrieved_arr;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}