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
//<ID> 523
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
    cJSON *flag_true = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *added_str = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_flag = (cJSON *)0;
    cJSON_bool is_bool = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    flag_true = cJSON_CreateTrue();
    flag_false = cJSON_CreateFalse();

    // step 3: Configure
    add_ok = cJSON_AddItemToArray(arr, flag_true);
    add_ok = cJSON_AddItemToArray(arr, flag_false);
    add_ok = cJSON_AddItemToObject(root, "flags", arr);
    added_str = cJSON_AddStringToObject(root, "title", "example");

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "flags");
    first_flag = cJSON_GetArrayItem(retrieved_arr, 0);
    is_bool = cJSON_IsBool((const cJSON *const)first_flag);

    // step 5: Validate
    (void)is_bool;
    (void)added_str;
    (void)add_ok;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}