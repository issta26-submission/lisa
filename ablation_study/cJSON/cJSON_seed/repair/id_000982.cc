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
//<ID> 982
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
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num_item);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_SetNumberHelper(num_item, 42.0);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *got = cJSON_GetArrayItem(arr, 1);
    char *retrieved = (char *)cJSON_GetStringValue(got);
    cJSON *copied = cJSON_CreateString(retrieved);
    cJSON_AddItemToObject(root, "copied", copied);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}