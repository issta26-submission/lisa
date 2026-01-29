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
//<ID> 548
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
    cJSON *null_item = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *first_in_arr = (cJSON *)0;
    const char *first_str = (const char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool is_arr = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello-world");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "greeting", str_item);
    add_ok2 = cJSON_AddItemToObject(root, "nothing", null_item);
    add_ok3 = cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    greeting = cJSON_GetObjectItem(root, "greeting");
    detached = cJSON_DetachItemViaPointer(root, greeting);
    (void)cJSON_AddItemToArray(arr, detached);
    is_arr = cJSON_IsArray(cJSON_GetObjectItem(root, "list"));
    first_in_arr = cJSON_GetArrayItem(arr, 0);
    first_str = cJSON_GetStringValue(first_in_arr);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)is_arr;
    (void)first_str;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}