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
//<ID> 158
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
    cJSON *true_item = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *json_text = (char *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool first_is_string = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("hello");
    true_item = cJSON_CreateTrue();
    str2 = cJSON_CreateString("world");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    detached = cJSON_DetachItemFromArray(arr, 1);
    first_is_string = cJSON_IsString(cJSON_GetArrayItem(arr, 0));
    cJSON_AddNumberToObject(root, "first_is_string", (double)first_is_string);

    // step 5: Validate
    size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}