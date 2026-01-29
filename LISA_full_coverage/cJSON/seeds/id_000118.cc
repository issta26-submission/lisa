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
//<ID> 118
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
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));

    // step 2: Configure
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON *meta_ref = cJSON_CreateObjectReference(meta_dup);
    cJSON_AddItemToObject(root, "meta_ref", meta_ref);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr_dup);
    cJSON_AddItemToObject(root, "items_ref", arr_ref);
    cJSON *newitem = cJSON_CreateString("replacement");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, newitem);

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    char *unfmt = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    const char *first_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    int summary = arr_size + (unfmt ? (int)unfmt[0] : 0) + (first_val ? (int)first_val[0] : 0) + (int)replaced + (int)root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unfmt);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}