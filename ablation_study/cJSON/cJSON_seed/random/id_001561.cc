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
//<ID> 1561
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
    cJSON *first_elem = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    char *first_str = (char *)0;
    cJSON_bool added_str_to_arr = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");

    // step 3: Configure
    added_str_to_arr = cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "list", arr);
    flag_item = cJSON_AddBoolToObject(root, "active", (cJSON_bool)1);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(first_elem);
    retrieved_flag = cJSON_GetObjectItem(root, "active");

    // step 5: Validate
    validation_score = (int)added_str_to_arr
        + (int)(arr_size == 1)
        + (int)(first_str != (char *)0)
        + (int)(retrieved_flag != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_str;
    (void)first_elem;
    (void)retrieved_arr;
    (void)flag_item;
    (void)str_item;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}