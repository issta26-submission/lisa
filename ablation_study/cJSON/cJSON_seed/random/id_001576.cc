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
//<ID> 1576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_elem0 = (cJSON *)0;
    cJSON *arr_elem1 = (cJSON *)0;
    cJSON *num_alpha = (cJSON *)0;
    cJSON *dup_child = (cJSON *)0;
    cJSON *dup_alpha = (cJSON *)0;
    int array_size = 0;
    cJSON_bool add_child_to_root = 0;
    cJSON_bool add_items_to_child = 0;
    cJSON_bool added_elem0 = 0;
    cJSON_bool added_elem1 = 0;
    cJSON_bool first_is_number = 0;
    cJSON_bool dup_alpha_is_number = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    num_alpha = cJSON_AddNumberToObject(child, "alpha", 1.2345);
    add_items_to_child = cJSON_AddItemToObject(child, "items", arr);
    added_elem0 = cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    added_elem1 = cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    add_child_to_root = cJSON_AddItemToObject(root, "child", child);
    (void)num_alpha;

    // step 4: Operate
    array_size = cJSON_GetArraySize(cJSON_GetObjectItem(child, "items"));
    arr_elem0 = cJSON_GetArrayItem(arr, 0);
    first_is_number = cJSON_IsNumber(arr_elem0);
    dup_child = cJSON_Duplicate(child, 1);
    dup_alpha = cJSON_GetObjectItem(dup_child, "alpha");
    dup_alpha_is_number = cJSON_IsNumber(dup_alpha);

    // step 5: Validate
    validation_score = (int)(array_size == 2)
        + (int)first_is_number
        + (int)(dup_child != NULL)
        + (int)dup_alpha_is_number
        + (int)add_items_to_child
        + (int)added_elem0
        + (int)added_elem1
        + (int)add_child_to_root;

    // step 6: Cleanup
    cJSON_Delete(dup_child);
    cJSON_Delete(root);
    (void)validation_score;
    (void)arr_elem0;
    (void)arr_elem1;
    (void)num_alpha;
    (void)dup_alpha;
    // API sequence test completed successfully
    return 66;
}