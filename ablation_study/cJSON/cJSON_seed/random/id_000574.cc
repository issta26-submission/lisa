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
//<ID> 574
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
    cJSON *arr_n1 = (cJSON *)0;
    cJSON *arr_n2 = (cJSON *)0;
    cJSON *arr_n3 = (cJSON *)0;
    cJSON *item_one = (cJSON *)0;
    cJSON *item_two = (cJSON *)0;
    cJSON *replacement_via_ptr = (cJSON *)0;
    cJSON *replacement_by_key = (cJSON *)0;
    cJSON *retrieved_one = (cJSON *)0;
    int arr_size = 0;
    double retrieved_value = 0.0;
    cJSON_bool add_ok_arr1 = 0;
    cJSON_bool add_ok_arr2 = 0;
    cJSON_bool add_ok_arr3 = 0;
    cJSON_bool add_ok_root_arr = 0;
    cJSON_bool add_ok_one = 0;
    cJSON_bool add_ok_two = 0;
    cJSON_bool replace_ok_ptr = 0;
    cJSON_bool replace_ok_key = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    arr_n1 = cJSON_CreateNumber(1.0);
    arr_n2 = cJSON_CreateNumber(2.0);
    arr_n3 = cJSON_CreateNumber(3.0);
    item_one = cJSON_CreateNumber(100.0);
    item_two = cJSON_CreateNumber(200.0);

    // step 3: Configure
    add_ok_arr1 = cJSON_AddItemToArray(arr, arr_n1);
    add_ok_arr2 = cJSON_AddItemToArray(arr, arr_n2);
    add_ok_arr3 = cJSON_AddItemToArray(arr, arr_n3);
    add_ok_root_arr = cJSON_AddItemToObject(root, "list", arr);
    add_ok_one = cJSON_AddItemToObject(root, "one", item_one);
    add_ok_two = cJSON_AddItemToObject(root, "two", item_two);

    // step 4: Operate
    replacement_via_ptr = cJSON_CreateNumber(111.0);
    replace_ok_ptr = cJSON_ReplaceItemViaPointer(root, item_two, replacement_via_ptr);
    replacement_by_key = cJSON_CreateNumber(222.0);
    replace_ok_key = cJSON_ReplaceItemInObject(root, "one", replacement_by_key);
    arr_size = cJSON_GetArraySize(arr);
    retrieved_one = cJSON_GetObjectItem(root, "one");
    retrieved_value = cJSON_GetNumberValue(retrieved_one);

    // step 5: Validate
    (void)add_ok_arr1;
    (void)add_ok_arr2;
    (void)add_ok_arr3;
    (void)add_ok_root_arr;
    (void)add_ok_one;
    (void)add_ok_two;
    (void)replace_ok_ptr;
    (void)replace_ok_key;
    (void)arr_size;
    (void)retrieved_one;
    (void)retrieved_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}