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
//<ID> 1566
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
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    char *printed = (char *)0;
    const char *first_str = (const char *)0;
    const char *flag_str = (const char *)0;
    cJSON_bool added_str_to_arr = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON *tmp_bool_obj = (cJSON *)0;
    cJSON_bool added_bool_to_obj = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");

    // step 3: Configure
    added_str_to_arr = cJSON_AddItemToArray(arr, str_item);
    added_arr_to_obj = cJSON_AddItemToObject(root, "data", arr);
    tmp_bool_obj = cJSON_AddBoolToObject(root, "flag", 1);
    added_bool_to_obj = (tmp_bool_obj != (cJSON *)0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(first_elem);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    flag_str = cJSON_GetStringValue(retrieved_flag);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_str_to_arr
        + (int)added_arr_to_obj
        + (int)added_bool_to_obj
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(arr_size == 1)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_str != (const char *)0)
        + (int)(retrieved_flag != (cJSON *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)flag_str;
    (void)first_str;
    (void)retrieved_arr;
    (void)first_elem;
    (void)retrieved_flag;
    (void)str_item;
    (void)arr;
    (void)tmp_bool_obj;
    // API sequence test completed successfully
    return 66;
}