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
//<ID> 128
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);

    // step 2: Configure
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_AddItemToArray(items, detached_meta);
    cJSON *subarr = cJSON_CreateArray();
    cJSON_AddItemToArray(subarr, cJSON_CreateString("inner"));
    cJSON_AddItemToObject(root, "sub", subarr);
    cJSON *sub_ref = cJSON_Duplicate(subarr, 1);
    cJSON_AddItemToArray(items, sub_ref);

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *printed = cJSON_Print(root);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    int arr_size = cJSON_GetArraySize(items);
    int summary = arr_size + (printed ? (int)printed[0] : 0) + (int)pre_ok + (int)items_is_array;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}