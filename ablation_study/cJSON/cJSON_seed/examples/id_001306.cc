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
//<ID> 1306
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
    cJSON *str1 = cJSON_CreateString("one");
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *str2 = cJSON_CreateString("two");
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddFalseToObject(meta, "enabled");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON_bool was_invalid = cJSON_IsInvalid(replacement);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    const cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first_item);
    size_t len = (printed != NULL) ? strlen(printed) : 0;
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    if (printed) memcpy(copy, printed, len + 1);
    (void)first_val;
    (void)was_invalid;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}