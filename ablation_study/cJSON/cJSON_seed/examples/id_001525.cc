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
//<ID> 1525
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_bool added_arr = cJSON_AddItemToObject(root, "values", arr);
    cJSON *n1 = cJSON_CreateNumber(123.0);
    cJSON *n2 = cJSON_CreateNumber(456.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_bool added_greeting = cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *bool_item = cJSON_CreateBool(1);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first);
    cJSON *greet_item = cJSON_GetObjectItem(root, "greeting");
    cJSON_bool greet_is_string = cJSON_IsString(greet_item);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, bool_item);
    char *printed = cJSON_PrintUnformatted(root);
    (void)added_arr;
    (void)added_greeting;
    (void)first_is_number;
    (void)greet_is_string;
    (void)replaced;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}