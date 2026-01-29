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
//<ID> 482
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
    cJSON *true_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    char *printed = (char *)0;
    const char *first_str = (const char *)0;
    cJSON_bool arr_check = 0;
    cJSON_bool added_bool = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "greeting", "world");
    cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);
    cJSON_AddFalseToObject(root, "always_false");

    // step 4: Operate
    arr_check = cJSON_IsArray((const cJSON *const)arr);
    first = cJSON_GetArrayItem(arr, 0);
    first_str = cJSON_GetStringValue((const cJSON *const)first);
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    added_bool = cJSON_IsArray((const cJSON *const)root); /* meaningful reuse of API result */

    // step 5: Validate
    (void)arr_check;
    (void)first_str;
    (void)added_bool;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}