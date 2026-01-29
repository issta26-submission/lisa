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
//<ID> 925
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(0.0);
    cJSON *elem_str = cJSON_CreateString("device-1");
    cJSON *greeting_old = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting_old);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, elem_str);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_SetNumberHelper(first_item, 42.5);
    cJSON *replacement = cJSON_CreateString("world");
    cJSON_ReplaceItemInObject(root, "greeting", replacement);

    // step 3: Operate & Validate
    cJSON *greeting = cJSON_GetObjectItem(root, "greeting");
    double copied_value = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "copied_value", copied_value);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}