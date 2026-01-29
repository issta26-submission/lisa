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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *str_item2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *got_inner = (cJSON *)0;
    cJSON *got_list = (cJSON *)0;
    char *printed = (char *)0;
    const char *greeting_val = (const char *)0;
    cJSON_bool is_list_array = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    str_item2 = cJSON_CreateString("world");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, str_item2);
    cJSON_AddItemToObject(inner, "list", arr);
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON_AddStringToObject(root, "greeting", "hi there");
    cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);
    cJSON_AddFalseToObject(root, "disabled");
    cJSON_AddItemToObject(root, "explicit_true", true_item);

    // step 4: Operate
    got_inner = cJSON_GetObjectItem(root, "inner");
    got_list = cJSON_GetObjectItem(got_inner, "list");
    is_list_array = cJSON_IsArray(got_list);
    greeting_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    (void)greeting_val;
    (void)is_list_array;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}