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
//<ID> 985
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *item0 = cJSON_CreateString("one");
    cJSON *item1 = cJSON_CreateString("two");
    cJSON *item2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(items, item0);
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToArray(items, item2);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    char *val_two = cJSON_GetStringValue(item1);
    cJSON *replacement = cJSON_CreateString(val_two);
    cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON *num_item = cJSON_GetArrayItem(items, 2);
    cJSON_SetNumberHelper(num_item, 42.0);
    cJSON *num_copy = cJSON_Duplicate(num_item, 0);
    cJSON_AddItemToObject(root, "num_copy", num_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}