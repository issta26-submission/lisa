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
//<ID> 2133
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
    cJSON *label = cJSON_AddStringToObject(root, "label", "initial_value");
    cJSON_SetValuestring(label, "updated_value");
    cJSON *entry1 = cJSON_CreateString("first");
    cJSON *entry2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, entry1);
    cJSON_AddItemToArray(arr, entry2);
    cJSON *n = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, n);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 1);
    const char *updated = cJSON_GetStringValue(label);
    cJSON_AddStringToObject(root, "copied_label", updated);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}