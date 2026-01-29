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
//<ID> 488
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
    char *printed = (char *)0;
    const char *first_value = (const char *)0;
    cJSON_bool is_array = 0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("member_one");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddStringToObject(root, "title", "example_title");
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddFalseToObject(root, "disabled");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    is_array = cJSON_IsArray(arr);
    arr_size = cJSON_GetArraySize(arr);
    first_value = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    (void)first_value;
    (void)is_array;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}