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
//<ID> 159
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *got0_before = (cJSON *)0;
    cJSON *got0_after = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    cJSON_bool first_was_string = (cJSON_bool)0;
    cJSON_bool first_is_string_after = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, str1);
    add_ok2 = cJSON_AddItemToArray(arr, str2);
    add_ok3 = cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "always_true", true_item);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    got0_before = cJSON_GetArrayItem(arr, 0);
    first_was_string = cJSON_IsString(got0_before);
    detached = cJSON_DetachItemFromArray(arr, 0);
    size_after = cJSON_GetArraySize(arr);
    cJSON_AddItemToArray(arr, detached);
    got0_after = cJSON_GetArrayItem(arr, 0);
    first_is_string_after = cJSON_IsString(got0_after);

    // step 5: Validate
    cJSON_AddBoolToObject(root, "first_was_string", first_was_string);
    cJSON_AddBoolToObject(root, "first_is_string_after", first_is_string_after);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}