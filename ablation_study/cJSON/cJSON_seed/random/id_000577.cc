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
//<ID> 577
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *replacement_num = (cJSON *)0;
    cJSON *status_old = (cJSON *)0;
    cJSON *status_new = (cJSON *)0;
    cJSON *arr_item_ptr = (cJSON *)0;
    int arr_size_before = 0;
    double replaced_value = 0.0;
    cJSON_bool add_ok = 0;
    cJSON_bool replace_ok1 = 0;
    cJSON_bool replace_ok2 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.5);
    num1 = cJSON_CreateNumber(2.5);
    status_old = cJSON_CreateString("old-status");
    status_new = cJSON_CreateString("new-status");

    // step 3: Configure
    add_ok = cJSON_AddItemToArray(arr, num0);
    add_ok = cJSON_AddItemToArray(arr, num1);
    add_ok = cJSON_AddItemToObject(root, "numbers", arr);
    add_ok = cJSON_AddItemToObject(root, "status", status_old);

    // step 4: Operate
    replace_ok1 = cJSON_ReplaceItemInObject(root, "status", status_new);
    arr_size_before = cJSON_GetArraySize(arr);
    arr_item_ptr = cJSON_GetArrayItem(arr, 1);
    replacement_num = cJSON_CreateNumber(99.99);
    replace_ok2 = cJSON_ReplaceItemViaPointer(arr, arr_item_ptr, replacement_num);
    replaced_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));

    // step 5: Validate
    (void)add_ok;
    (void)replace_ok1;
    (void)replace_ok2;
    (void)arr_size_before;
    (void)replaced_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}