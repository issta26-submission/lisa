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
//<ID> 397
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
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate and Validate
    cJSON_InsertItemInArray(arr, 1, inserted);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_SetValuestring(detached, "modified");
    const char *modified_val = cJSON_GetStringValue(detached);
    cJSON *new_copy = cJSON_CreateString(modified_val);
    int current_size = cJSON_GetArraySize(arr);
    cJSON_InsertItemInArray(arr, current_size, new_copy);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_bool items_equal = cJSON_Compare(new_copy, detached, 1);
    cJSON_AddNumberToObject(root, "final_size", (double)cJSON_GetArraySize(arr));
    cJSON_AddBoolToObject(root, "items_equal", items_equal);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}