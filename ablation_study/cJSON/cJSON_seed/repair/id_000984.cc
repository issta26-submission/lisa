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
//<ID> 984
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
    cJSON *first = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, first);
    cJSON *second = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, second);
    cJSON *num_item = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    cJSON *copied = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "copied_first", copied);
    double updated = cJSON_SetNumberHelper(num_item, 99.0);
    cJSON *replacement = cJSON_CreateNumber(updated);
    cJSON_ReplaceItemInArray(items, 2, replacement);
    char *verify = cJSON_GetStringValue(copied);
    (void)verify;
    (void)updated;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}