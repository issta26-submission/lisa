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
//<ID> 694
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
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *new_greeting = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    cJSON *verify_greeting = (cJSON *)0;
    char *greeting_value = (char *)0;
    cJSON_bool is_array = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool replaced = 0;
    int presence_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.0);
    str = cJSON_CreateString("answer");
    greeting = cJSON_CreateString("hello");
    new_greeting = cJSON_CreateString("hi");

    // step 3: Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    added_arr_to_obj = cJSON_AddItemToObject(root, "list", arr);
    added_ref = cJSON_AddItemReferenceToObject(root, "list_ref", arr);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 4: Operate
    retrieved_list = cJSON_GetObjectItem(root, "list");
    is_array = cJSON_IsArray(retrieved_list);
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    replaced = cJSON_ReplaceItemViaPointer(root, retrieved_greeting, new_greeting);
    verify_greeting = cJSON_GetObjectItem(root, "greeting");
    greeting_value = cJSON_GetStringValue(verify_greeting);

    // step 5: Validate
    presence_count = (is_array != 0) + (added_arr_to_obj != 0) + (added_ref != 0) + (replaced != 0) + (greeting_value != (char *)0);
    (void)presence_count;
    (void)retrieved_list;
    (void)retrieved_greeting;
    (void)verify_greeting;
    (void)greeting_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}