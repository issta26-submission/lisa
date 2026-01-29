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
//<ID> 213
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
    cJSON *ver_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *is_array_item = (cJSON *)0;
    cJSON *elem_num = (cJSON *)0;
    cJSON *elem_str = (cJSON *)0;
    cJSON *elem_false = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    cJSON_bool is_arr_flag = (cJSON_bool)0;
    char *json_text = (char *)0;
    const char *ver_text = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure
    ver_text = cJSON_Version();
    ver_item = cJSON_CreateString(ver_text);
    cJSON_AddItemToObject(root, "cjson_version", ver_item);
    true_item = cJSON_AddTrueToObject(root, "always_true");
    is_arr_flag = cJSON_IsArray(arr);
    is_array_item = cJSON_CreateBool(is_arr_flag);
    cJSON_AddItemToObject(root, "values_is_array", is_array_item);

    // step 4: Operate
    elem_num = cJSON_CreateNumber(42.0);
    elem_str = cJSON_CreateString("example");
    elem_false = cJSON_CreateFalse();
    add_ok1 = cJSON_AddItemToArray(arr, elem_num);
    add_ok2 = cJSON_AddItemToArray(arr, elem_str);
    add_ok3 = cJSON_AddItemToArray(arr, elem_false);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}