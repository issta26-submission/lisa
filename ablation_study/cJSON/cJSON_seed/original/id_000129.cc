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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));

    // step 2: Configure
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON *dup_meta = cJSON_Duplicate(detached_meta, 1);
    cJSON_AddItemToArray(arr, dup_meta);
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddStringToObject(extra, "tag", "x");
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    int summary = (printed ? (int)printed[0] : 0) + (int)pre_ok + (int)arr_is_array + arr->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached_meta);

    // API sequence test completed successfully
    return 66;
}