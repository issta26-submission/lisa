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
//<ID> 1589
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
    cJSON *dup_arr = (cJSON *)0;
    cJSON *str_a = (cJSON *)0;
    cJSON *str_b = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_copy = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_str_val = (const char *)0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_dup_to_root = 0;
    int size_list = 0;
    int size_copy = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_a = cJSON_CreateString("alpha");
    str_b = cJSON_CreateString("beta");

    // step 3: Configure
    added_a = cJSON_AddItemToArray(arr, str_a);
    added_b = cJSON_AddItemToArray(arr, str_b);
    dup_arr = cJSON_Duplicate(arr, 1);
    added_arr_to_root = cJSON_AddItemToObjectCS(root, "list", arr);
    added_dup_to_root = cJSON_AddItemToObjectCS(root, "list_copy", dup_arr);

    // step 4: Operate
    retrieved_list = cJSON_GetObjectItem(root, "list");
    retrieved_copy = cJSON_GetObjectItem(root, "list_copy");
    size_list = cJSON_GetArraySize(retrieved_list);
    size_copy = cJSON_GetArraySize(retrieved_copy);
    first_elem = cJSON_GetArrayItem(retrieved_list, 0);
    first_str_val = cJSON_GetStringValue(first_elem);

    // step 5: Validate
    validation_score = (int)added_a
        + (int)added_b
        + (int)added_arr_to_root
        + (int)added_dup_to_root
        + (int)(size_list == 2)
        + (int)(size_copy == 2)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_str_val != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_copy;
    (void)retrieved_list;
    (void)size_list;
    (void)size_copy;
    (void)first_str_val;
    // API sequence test completed successfully
    return 66;
}