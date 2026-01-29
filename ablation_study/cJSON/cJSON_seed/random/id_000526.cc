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
//<ID> 526
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
    cJSON *str_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *added_str_ptr = (cJSON *)0;
    cJSON_bool add_ok_arr1 = 0;
    cJSON_bool add_ok_arr2 = 0;
    cJSON_bool add_ok_obj = 0;
    cJSON_bool is_bool = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("world");
    bool_item = cJSON_CreateTrue();

    // step 3: Configure
    added_str_ptr = cJSON_AddStringToObject(root, "message", "hello");
    add_ok_arr1 = cJSON_AddItemToArray(arr, bool_item);
    add_ok_arr2 = cJSON_AddItemToArray(arr, str_item);
    add_ok_obj = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    is_bool = cJSON_IsBool((const cJSON *const)bool_item);

    // step 5: Validate
    (void)added_str_ptr;
    (void)add_ok_arr1;
    (void)add_ok_arr2;
    (void)add_ok_obj;
    (void)is_bool;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}