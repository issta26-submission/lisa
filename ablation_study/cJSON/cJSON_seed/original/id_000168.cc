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
//<ID> 168
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
    cJSON *b0 = cJSON_CreateTrue();
    cJSON *b1 = cJSON_CreateFalse();
    cJSON_InsertItemInArray(arr, 0, b0);
    cJSON_InsertItemInArray(arr, 1, b1);
    cJSON_AddItemToObject(root, "flags", arr);

    // step 2: Configure
    cJSON *group = cJSON_AddObjectToObject(root, "group");
    cJSON_AddTrueToObject(group, "always_enabled");
    cJSON *b2 = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 1, b2);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 2);
    cJSON_bool detached_is_bool = cJSON_IsBool(detached);
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    int summary = (int)first_is_bool + (int)detached_is_bool + root->type + arr->type + (int)buffer[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}