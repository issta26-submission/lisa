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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "items", arr);
    cJSON *t1 = cJSON_CreateTrue();
    cJSON *f1 = cJSON_CreateFalse();
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 0, t1);
    cJSON_InsertItemInArray(arr, 1, f1);
    cJSON_InsertItemInArray(arr, 2, t2);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "verbose");
    char *buffer = (char *)cJSON_malloc((size_t)256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_bool detached_is_bool = cJSON_IsBool(detached);
    cJSON_InsertItemInArray(arr, 0, detached);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first);
    int summary = (int)detached_is_bool + (int)first_is_bool + (buffer ? (int)buffer[0] : 0) + root->type + cfg->type + arr->type + meta->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}