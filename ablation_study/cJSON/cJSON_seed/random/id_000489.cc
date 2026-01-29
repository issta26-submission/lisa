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
//<ID> 489
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
    cJSON *greeting = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *sval = (const char *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool add_ok = 0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("first");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    add_ok = cJSON_AddItemToArray(arr, str_item);
    add_ok = cJSON_AddItemToArray(arr, cJSON_CreateString("second"));
    add_ok = cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "list", arr);
    greeting = cJSON_AddStringToObject(root, "greeting", "hello");
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddFalseToObject(root, "disabled");

    // step 4: Operate
    is_arr = cJSON_IsArray((const cJSON *const)arr);
    arr_size = cJSON_GetArraySize(arr);
    sval = cJSON_GetStringValue((const cJSON *const)greeting);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)add_ok;
    (void)is_arr;
    (void)arr_size;
    (void)sval;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}