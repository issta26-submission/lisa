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
//<ID> 579
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
    cJSON *num3 = (cJSON *)0;
    cJSON *old_str = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *arr_first = (cJSON *)0;
    cJSON *replacement_via_ptr = (cJSON *)0;
    cJSON *replacement_in_obj = (cJSON *)0;
    int arr_size = 0;
    double first_value = 0.0;
    cJSON_bool added_ok1 = 0;
    cJSON_bool added_ok2 = 0;
    cJSON_bool added_ok3 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    num3 = cJSON_CreateNumber(3.0);
    old_str = cJSON_CreateString("old_value");

    // step 3: Configure
    added_ok1 = cJSON_AddItemToArray(arr, num1);
    added_ok2 = cJSON_AddItemToArray(arr, num2);
    added_ok3 = cJSON_AddItemToArray(arr, num3);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "replace_me", old_str);
    cJSON_AddItemToObject(root, "marker", cJSON_CreateNumber(0.0));

    // step 4: Operate
    retr_arr = cJSON_GetObjectItem(root, "array");
    arr_first = cJSON_GetArrayItem(retr_arr, 0);
    replacement_via_ptr = cJSON_CreateNumber(10.0);
    cJSON_ReplaceItemViaPointer(retr_arr, arr_first, replacement_via_ptr);
    replacement_in_obj = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInObject(root, "replace_me", replacement_in_obj);
    arr_size = cJSON_GetArraySize(retr_arr);
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(retr_arr, 0));

    // step 5: Validate
    (void)added_ok1;
    (void)added_ok2;
    (void)added_ok3;
    (void)arr_size;
    (void)first_value;
    (void)replacement_via_ptr;
    (void)replacement_in_obj;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}