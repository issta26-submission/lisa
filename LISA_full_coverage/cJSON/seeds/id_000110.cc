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
//<ID> 110
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s1);

    // step 2: Configure (use duplicates instead of references to avoid ownership/double-free issues)
    cJSON *meta_ref = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_ref", meta_ref);
    cJSON *arr_ref = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_ref", arr_ref);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_ReplaceItemInArray(arr, 1, s2);

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_AddItemToArray(arr, detached_meta);
    char *unfmt = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    int summary = arr_size + (unfmt ? (int)unfmt[0] : 0) + (int)buffer[0] + (int)meta_ref->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unfmt);
    cJSON_Delete(root);

    return 66;
}