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
//<ID> 1303
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
    cJSON *item0 = cJSON_CreateString("one");
    cJSON *item1 = cJSON_CreateNumber(2.0);
    cJSON *item2 = cJSON_CreateString("three");
    cJSON *replacement = cJSON_CreateString("replaced");

    // step 2: Configure
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *flag_false = cJSON_AddFalseToObject(root, "active");

    // step 3: Operate & Validate
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *val = cJSON_GetStringValue(first);
    size_t len = strlen(val);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, val, len + 1);
    cJSON_bool replaced_invalid = cJSON_IsInvalid(cJSON_GetArrayItem(arr, 1));
    cJSON_bool flag_invalid = cJSON_IsInvalid(flag_false);
    (void)replaced_invalid;
    (void)flag_invalid;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}