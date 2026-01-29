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
//<ID> 521
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
    cJSON *name_item = (cJSON *)0;
    cJSON *arr_string = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    const char *name_str = (const char *)0;
    cJSON_bool is_bool = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    name_item = cJSON_AddStringToObject(root, "name", "example");
    arr_string = cJSON_CreateString("entry");
    add_ok = cJSON_AddItemToArray(arr, arr_string);
    bool_item = cJSON_CreateTrue();
    add_ok = cJSON_AddItemToArray(arr, bool_item);
    add_ok = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_bool = cJSON_IsBool((const cJSON *const)retrieved);
    name_str = cJSON_GetStringValue(name_item);

    // step 5: Validate
    (void)is_bool;
    (void)name_str;
    (void)add_ok;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}