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
//<ID> 483
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
    cJSON *first = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    cJSON_bool arr_is_array = 0;
    cJSON_bool is_true = 0;
    const char *retrieved_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "name", "example_name");
    cJSON_AddFalseToObject(root, "disabled");
    arr_is_array = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "items_is_array", arr_is_array);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    is_true = cJSON_IsTrue(first);
    retrieved_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    (void)arr_size;
    (void)is_true;
    (void)retrieved_str;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}