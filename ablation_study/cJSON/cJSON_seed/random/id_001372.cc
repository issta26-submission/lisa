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
//<ID> 1372
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
    cJSON *child_obj = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_child = (cJSON *)0;
    cJSON *greet_item = (cJSON *)0;
    char *json_str = (char *)0;
    const char *first_elem_str = (const char *)0;
    int is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToObject(root, "numbers", arr);
    child_obj = cJSON_CreateObject();
    greet_item = cJSON_AddStringToObject(child_obj, "greet", "hello");
    cJSON_AddItemToObject(root, "child", child_obj);

    // step 3: Configure
    cJSON_AddStringToObject(root, "status", "ready");

    // step 4: Operate
    found_arr = cJSON_GetObjectItem(root, "numbers");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    item0 = cJSON_GetArrayItem(found_arr, 0);
    first_elem_str = cJSON_GetStringValue(item0);
    found_child = cJSON_GetObjectItem(root, "child");
    greet_item = cJSON_GetObjectItem(found_child, "greet");
    json_str = cJSON_Print(root);
    cJSON_Minify(json_str);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(item0 != (cJSON *)0) + (int)(first_elem_str != (const char *)0) + (int)(greet_item != (cJSON *)0) + (int)(json_str != (char *)0);

    // step 6: Cleanup
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)first_elem_str;
    return 66;
}