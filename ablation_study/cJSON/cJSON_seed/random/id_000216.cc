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
//<ID> 216
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
    cJSON *ver_str = (cJSON *)0;
    cJSON *num_from_isarray = (cJSON *)0;
    cJSON *elem_num = (cJSON *)0;
    cJSON *elem_str = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    const char *version_text = (const char *)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    version_text = cJSON_Version();
    ver_str = cJSON_CreateString(version_text);
    cJSON_AddItemToObject(root, "cjson_version", ver_str);
    true_item = cJSON_AddTrueToObject(root, "feature_enabled");

    // step 4: Operate
    elem_num = cJSON_CreateNumber(42.0);
    add_ok1 = cJSON_AddItemToArray(arr, elem_num);
    elem_str = cJSON_CreateString("element_one");
    add_ok2 = cJSON_AddItemToArray(arr, elem_str);
    is_arr = cJSON_IsArray(arr);
    num_from_isarray = cJSON_CreateNumber((double)is_arr);
    cJSON_AddItemToObject(root, "is_items_array", num_from_isarray);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}