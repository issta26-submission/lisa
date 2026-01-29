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
//<ID> 337
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
    cJSON *first_elem = (cJSON *)0;
    char *first_value = (char *)0;
    cJSON_bool first_is_string = (cJSON_bool)0;
    cJSON_bool root_is_object = (cJSON_bool)0;
    cJSON *added_back = (cJSON *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);

    // step 4: Operate
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetStringValue(first_elem);
    first_is_string = cJSON_IsString(first_elem);
    root_is_object = cJSON_IsObject(root);

    // step 5: Validate
    added_back = cJSON_AddStringToObject(root, "first_value", first_value);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}