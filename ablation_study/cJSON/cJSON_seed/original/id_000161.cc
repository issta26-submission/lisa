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
//<ID> 161
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
    cJSON *t1 = cJSON_CreateTrue();
    cJSON *f1 = cJSON_CreateFalse();
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, t1);
    cJSON_AddItemToArray(items, f1);
    cJSON_AddItemToArray(items, t2);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "enabled");
    cJSON *to_insert = cJSON_CreateBool(1);
    cJSON_InsertItemInArray(items, 1, to_insert);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first);
    int size_before = cJSON_GetArraySize(items);
    cJSON *detached = cJSON_DetachItemFromArray(items, 2);
    int size_after = cJSON_GetArraySize(items);
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));
    cJSON_PrintPreallocated(root, prebuf, 256, 1);
    int summary = (int)first_is_bool + size_before + size_after + detached->type + meta->type;
    (void)summary;
    cJSON_Delete(detached);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}