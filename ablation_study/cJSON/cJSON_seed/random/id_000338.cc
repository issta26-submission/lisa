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
//<ID> 338
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
    cJSON *obj = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *str_item2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    char *got_str = (char *)0;
    cJSON_bool is_first_str = (cJSON_bool)0;
    cJSON_bool is_root_obj = (cJSON_bool)0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    obj = cJSON_CreateObject();

    // step 3: Configure
    str_item = cJSON_CreateString("first_element");
    str_item2 = cJSON_CreateString("second_element");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, str_item2);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "metadata", obj);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddStringToObject(obj, "note", "example_note");

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 0);
    is_first_str = cJSON_IsString(got_item);
    is_root_obj = cJSON_IsObject(root);
    got_str = cJSON_GetStringValue(got_item);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate / propagate results
    cJSON_AddItemToObject(obj, "copied_first", cJSON_CreateString(got_str));
    cJSON_AddItemToObject(obj, "is_first_string", cJSON_CreateBool(is_first_str));
    cJSON_AddItemToObject(root, "root_is_object", cJSON_CreateBool(is_root_obj));
    cJSON_AddNumberToObject(root, "list_size", (double)arr_size);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}