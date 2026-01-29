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
//<ID> 1300
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
    cJSON *old_item = cJSON_CreateString("old_value");
    cJSON *new_item = cJSON_CreateString("new_value");

    // step 2: Configure
    cJSON_AddItemToArray(arr, old_item);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, new_item);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool invalid_flag = cJSON_IsInvalid(first);
    const char *str = cJSON_GetStringValue(first);
    size_t len = strlen(str);
    char *copy = (char *)cJSON_malloc(len + 1);
    memcpy(copy, str, len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}