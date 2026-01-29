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
//<ID> 573
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
    cJSON *flag_true = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_flag_ok = 0;
    cJSON_bool replace_ok1 = 0;
    cJSON_bool replace_ok2 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    flag_true = cJSON_CreateTrue();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    add_ok3 = cJSON_AddItemToObject(root, "numbers", arr);
    add_flag_ok = cJSON_AddItemToObject(root, "active", flag_true);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    new_num = cJSON_CreateNumber(99.0);
    replace_ok1 = cJSON_ReplaceItemInObject(root, "numbers", new_num);
    flag_false = cJSON_CreateFalse();
    replace_ok2 = cJSON_ReplaceItemViaPointer(root, flag_true, flag_false);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_flag_ok;
    (void)replace_ok1;
    (void)replace_ok2;
    (void)arr_size;
    (void)new_num;
    (void)flag_false;
    (void)num1;
    (void)num2;
    (void)flag_true;
    (void)arr;
    (void)root;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}