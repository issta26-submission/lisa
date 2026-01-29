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
//<ID> 1617
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
    cJSON_AddItemToObject(root, "fruits", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("apple"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("banana"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("cherry"));

    // step 2: Configure
    cJSON *new_item = cJSON_CreateString("date");
    cJSON_InsertItemInArray(arr, 1, new_item);
    cJSON *replacement = cJSON_CreateString("blueberry");
    cJSON_ReplaceItemInArray(arr, 2, replacement);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    char *detached_val = cJSON_GetStringValue(detached);
    cJSON *detached_copy = cJSON_CreateString(detached_val);
    cJSON_AddItemToArray(arr, detached_copy);
    cJSON_Delete(detached);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}