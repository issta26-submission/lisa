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
//<ID> 527
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
    cJSON *true_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *greeting_item = (cJSON *)0;
    cJSON *first_in_arr = (cJSON *)0;
    cJSON *second_in_arr = (cJSON *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool is_first_bool = 0;
    cJSON_bool is_second_bool = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "list", arr);
    add_ok1 = cJSON_AddItemToArray(arr, true_item);
    add_ok2 = cJSON_AddItemToArray(arr, false_item);
    greeting_item = cJSON_AddStringToObject(root, "greeting", "hello");

    // step 4: Operate
    first_in_arr = cJSON_GetArrayItem(arr, 0);
    second_in_arr = cJSON_GetArrayItem(arr, 1);
    is_first_bool = cJSON_IsBool((const cJSON *const)first_in_arr);
    is_second_bool = cJSON_IsBool((const cJSON *const)second_in_arr);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)greeting_item;
    (void)is_first_bool;
    (void)is_second_bool;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}