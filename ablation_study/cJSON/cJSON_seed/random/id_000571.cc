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
//<ID> 571
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
    cJSON *flag = (cJSON *)0;
    cJSON *new_flag = (cJSON *)0;
    cJSON *replacement_for_first = (cJSON *)0;
    cJSON *retrieved0 = (cJSON *)0;
    int arr_size = 0;
    double value0 = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok_obj1 = 0;
    cJSON_bool add_ok_obj2 = 0;
    cJSON_bool replace_ok_obj = 0;
    cJSON_bool replace_ok_ptr = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    flag = cJSON_CreateNumber(100.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    add_ok_obj1 = cJSON_AddItemToObject(root, "numbers", arr);
    add_ok_obj2 = cJSON_AddItemToObject(root, "replace_me", flag);

    // step 4: Operate
    new_flag = cJSON_CreateNumber(777.0);
    replace_ok_obj = cJSON_ReplaceItemInObject(root, "replace_me", new_flag);
    replacement_for_first = cJSON_CreateNumber(111.0);
    replace_ok_ptr = cJSON_ReplaceItemViaPointer(arr, num1, replacement_for_first);
    arr_size = cJSON_GetArraySize(arr);
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    value0 = cJSON_GetNumberValue(retrieved0);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok_obj1;
    (void)add_ok_obj2;
    (void)replace_ok_obj;
    (void)replace_ok_ptr;
    (void)arr_size;
    (void)value0;
    (void)retrieved0;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}