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
//<ID> 578
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
    cJSON *n3 = (cJSON *)0;
    cJSON *obj_old = (cJSON *)0;
    cJSON *obj_new = (cJSON *)0;
    cJSON *replacement_num = (cJSON *)0;
    int arr_size = 0;
    double first_value = 0.0;
    cJSON_bool ok_add_arr = 0;
    cJSON_bool ok_add_obj = 0;
    cJSON_bool ok_replace_obj = 0;
    cJSON_bool ok_replace_via_ptr = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.0);
    n2 = cJSON_CreateNumber(2.0);
    n3 = cJSON_CreateNumber(3.0);
    obj_old = cJSON_CreateNumber(100.0);

    // step 3: Configure
    ok_add_arr = cJSON_AddItemToArray(arr, n1);
    ok_add_arr = cJSON_AddItemToArray(arr, n2);
    ok_add_arr = cJSON_AddItemToArray(arr, n3);
    ok_add_obj = cJSON_AddItemToObject(root, "list", arr);
    ok_add_obj = cJSON_AddItemToObject(root, "replace_me", obj_old);
    obj_new = cJSON_CreateString("replaced");
    ok_replace_obj = cJSON_ReplaceItemInObject(root, "replace_me", obj_new);
    replacement_num = cJSON_CreateNumber(99.0);
    ok_replace_via_ptr = cJSON_ReplaceItemViaPointer(arr, n2, replacement_num);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));

    // step 5: Validate
    (void)ok_add_arr;
    (void)ok_add_obj;
    (void)ok_replace_obj;
    (void)ok_replace_via_ptr;
    (void)arr_size;
    (void)first_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}