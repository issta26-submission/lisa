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
//<ID> 575
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *itemA = (cJSON *)0;
    cJSON *itemB = (cJSON *)0;
    cJSON *first_in_arr = (cJSON *)0;
    cJSON *replacement_for_A = (cJSON *)0;
    cJSON *replacement_for_B = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool ok_add_arr1 = 0;
    cJSON_bool ok_add_arr2 = 0;
    cJSON_bool ok_add_root_arr = 0;
    cJSON_bool ok_add_itemA = 0;
    cJSON_bool ok_add_itemB = 0;
    cJSON_bool ok_replace_A_via_ptr = 0;
    cJSON_bool ok_replace_B_in_obj = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    itemA = cJSON_CreateString("alpha");
    itemB = cJSON_CreateString("beta");

    // step 3: Configure
    ok_add_arr1 = cJSON_AddItemToArray(arr, num1);
    ok_add_arr2 = cJSON_AddItemToArray(arr, num2);
    ok_add_root_arr = cJSON_AddItemToObject(root, "numbers", arr);
    ok_add_itemA = cJSON_AddItemToObject(root, "itemA", itemA);
    ok_add_itemB = cJSON_AddItemToObject(root, "itemB", itemB);

    // step 4: Operate
    first_in_arr = cJSON_GetArrayItem(arr, 0);
    replacement_for_A = cJSON_CreateString("alpha_replaced");
    ok_replace_A_via_ptr = cJSON_ReplaceItemViaPointer(arr, first_in_arr, replacement_for_A);
    replacement_for_B = cJSON_CreateNumber(42.0);
    ok_replace_B_in_obj = cJSON_ReplaceItemInObject(root, "itemB", replacement_for_B);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)ok_add_arr1;
    (void)ok_add_arr2;
    (void)ok_add_root_arr;
    (void)ok_add_itemA;
    (void)ok_add_itemB;
    (void)ok_replace_A_via_ptr;
    (void)ok_replace_B_in_obj;
    (void)first_in_arr;
    (void)replacement_for_A;
    (void)replacement_for_B;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}