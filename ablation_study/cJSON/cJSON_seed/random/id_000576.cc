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
//<ID> 576
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *replacement_arr = (cJSON *)0;
    cJSON *replacement_elem = (cJSON *)0;
    cJSON *new_value = (cJSON *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool replace_ok = 0;
    cJSON_bool replace_via_ptr_ok = 0;
    int arr_size_before = 0;
    int arr_size_after = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.0);
    n2 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, n1);
    add_ok2 = cJSON_AddItemToArray(arr, n2);
    add_ok3 = cJSON_AddItemToObject(root, "nums", arr);
    cJSON_AddItemToObject(root, "replace_me", cJSON_CreateNumber(100.0));

    // step 4: Operate
    arr_size_before = cJSON_GetArraySize(arr);
    new_value = cJSON_CreateString("replaced");
    replace_ok = cJSON_ReplaceItemInObject(root, "replace_me", new_value);
    replacement_arr = cJSON_CreateArray();
    replacement_elem = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(replacement_arr, replacement_elem);
    replace_via_ptr_ok = cJSON_ReplaceItemViaPointer(root, arr, replacement_arr);
    arr_size_after = cJSON_GetArraySize(replacement_arr);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)replace_ok;
    (void)replace_via_ptr_ok;
    (void)arr_size_before;
    (void)arr_size_after;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}