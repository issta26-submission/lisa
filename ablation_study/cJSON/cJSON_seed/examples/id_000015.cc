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
//<ID> 15
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str1);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    cJSON *countItem = cJSON_AddNumberToObject(root, "count", 2.0);
    (void)countItem;
    cJSON *replacement = cJSON_CreateString("beta");

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(null_item);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    const char *repl_value = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    (void)was_null;
    (void)replaced;
    (void)equal;
    (void)repl_value;

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}