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
//<ID> 394
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
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON *item_c = cJSON_CreateString("gamma");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, item_a);
    cJSON_InsertItemInArray(arr, 0, item_b);
    cJSON_InsertItemInArray(arr, 2, item_c);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_SetValuestring(detached, "beta-modified");
    cJSON_InsertItemInArray(arr, 1, detached);
    const char *first_item = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    (void)first_item;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}