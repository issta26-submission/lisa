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
//<ID> 1583
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
    cJSON *str_a = (cJSON *)0;
    cJSON *str_b = (cJSON *)0;
    cJSON *dup_b = (cJSON *)0;
    cJSON_bool added_a_to_arr = 0;
    cJSON_bool added_dup_to_arr = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    char *first_str = (char *)0;
    char *second_str = (char *)0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_a = cJSON_CreateString("alpha");
    str_b = cJSON_CreateString("beta");

    // step 3: Configure
    added_a_to_arr = cJSON_AddItemToArray(arr, str_a);
    dup_b = cJSON_Duplicate((const cJSON *)str_b, 1);
    added_dup_to_arr = cJSON_AddItemToArray(arr, dup_b);
    added_arr_to_root = cJSON_AddItemToObjectCS(root, "items", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    second_elem = cJSON_GetArrayItem(retrieved_arr, 1);
    first_str = cJSON_GetStringValue(first_elem);
    second_str = cJSON_GetStringValue(second_elem);

    // step 5: Validate
    validation_score = (int)added_a_to_arr
        + (int)added_dup_to_arr
        + (int)added_arr_to_root
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_size == 2)
        + (int)(first_elem != (cJSON *)0)
        + (int)(second_elem != (cJSON *)0)
        + (int)(first_str != (char *)0)
        + (int)(second_str != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)str_b;
    (void)first_str;
    (void)second_str;
    (void)first_elem;
    (void)second_elem;
    (void)retrieved_arr;
    (void)dup_b;
    // API sequence test completed successfully
    return 66;
}