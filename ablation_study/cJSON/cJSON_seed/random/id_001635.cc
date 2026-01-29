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
//<ID> 1635
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *count_node = (cJSON *)0;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_str2 = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool is_str1 = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("first");
    str2 = cJSON_CreateString("second");

    // step 3: Configure
    added_str1 = cJSON_AddItemToArray(arr, str1);
    added_str2 = cJSON_AddItemToArray(arr, str2);
    added_arr_to_root = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    count_node = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    is_str1 = cJSON_IsString(str1);

    // step 5: Validate
    validation_score = (int)added_str1
        + (int)added_str2
        + (int)added_arr_to_root
        + (int)(arr_size == 2)
        + (int)(count_node != (cJSON *)0)
        + (int)is_str1;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)str2;
    (void)str1;
    (void)count_node;
    (void)added_str1;
    (void)added_str2;
    (void)added_arr_to_root;
    (void)is_str1;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}