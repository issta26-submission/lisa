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
//<ID> 480
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
    cJSON *obj_enabled = (cJSON *)0;
    cJSON *obj_disabled = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_arr = 0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("item1");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddStringToObject(root, "status", "running");
    obj_enabled = cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);
    obj_disabled = cJSON_AddFalseToObject(root, "disabled");

    // step 4: Operate
    is_arr = cJSON_IsArray((const cJSON *const)arr);
    arr_size = cJSON_GetArraySize((const cJSON *const)arr);
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate
    (void)is_arr;
    (void)arr_size;
    (void)obj_enabled;
    (void)obj_disabled;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}