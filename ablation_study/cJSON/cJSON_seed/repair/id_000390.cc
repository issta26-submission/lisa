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
//<ID> 390
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
    cJSON *item1 = cJSON_CreateString("one");
    cJSON *item2 = cJSON_CreateString("two");
    cJSON *item3 = cJSON_CreateString("three");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_InsertItemInArray(arr, 0, item1);
    cJSON_InsertItemInArray(arr, 1, item2);
    cJSON_InsertItemInArray(arr, 2, item3);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    char *updated = cJSON_SetValuestring(detached, "two-updated");
    cJSON_AddStringToObject(root, "detached_value", updated);
    cJSON_InsertItemInArray(arr, 1, detached);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}