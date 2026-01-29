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
//<ID> 1531
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
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_first = (cJSON *)0;
    char *retrieved_str = (char *)0;
    cJSON_bool added_to_array1 = 0;
    cJSON_bool added_to_array2 = 0;
    cJSON_bool added_arr_to_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    num_item = cJSON_CreateNumber(100.0);

    // step 3: Configure
    added_to_array1 = cJSON_AddItemToArray(arr, str_item);
    added_to_array2 = cJSON_AddItemToArray(arr, num_item);
    added_arr_to_object = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_first = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_str = (char *)cJSON_GetStringValue(retrieved_first);

    // step 5: Validate
    validation_score = (int)added_to_array1 + (int)added_to_array2 + (int)added_arr_to_object + (int)(retrieved_arr != (cJSON *)0) + (int)(retrieved_first != (cJSON *)0) + (int)(retrieved_str != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_str;
    (void)retrieved_first;
    (void)retrieved_arr;
    // API sequence test completed successfully
    return 66;
}