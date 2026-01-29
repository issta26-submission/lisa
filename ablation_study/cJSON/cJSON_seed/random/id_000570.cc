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
//<ID> 570
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
    cJSON *replacement_arr = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *label_repl = (cJSON *)0;
    cJSON *arr_n0 = (cJSON *)0;
    cJSON *arr_n1 = (cJSON *)0;
    cJSON *arr_repl_n0 = (cJSON *)0;
    int original_size = 0;
    int new_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool add_ok5 = 0;
    cJSON_bool replaced_via_pointer_ok = 0;
    cJSON_bool replaced_in_object_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    replacement_arr = cJSON_CreateArray();
    label = cJSON_CreateString("initial_label");
    label_repl = cJSON_CreateString("replaced_label");
    arr_n0 = cJSON_CreateNumber(1.0);
    arr_n1 = cJSON_CreateNumber(2.0);
    arr_repl_n0 = cJSON_CreateNumber(100.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, arr_n0);
    add_ok2 = cJSON_AddItemToArray(arr, arr_n1);
    add_ok3 = cJSON_AddItemToObject(root, "numbers", arr);
    add_ok4 = cJSON_AddItemToObject(root, "label", label);
    add_ok5 = cJSON_AddItemToArray(replacement_arr, arr_repl_n0);

    // step 4: Operate
    original_size = cJSON_GetArraySize(arr);
    replaced_via_pointer_ok = cJSON_ReplaceItemViaPointer(root, label, label_repl);
    replaced_in_object_ok = cJSON_ReplaceItemInObject(root, "numbers", replacement_arr);
    new_size = cJSON_GetArraySize(replacement_arr);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)add_ok5;
    (void)replaced_via_pointer_ok;
    (void)replaced_in_object_ok;
    (void)original_size;
    (void)new_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}