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
//<ID> 119
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

    // step 2: Configure
    cJSON *subarr = cJSON_CreateArray();
    cJSON_AddItemToArray(subarr, cJSON_CreateString("inner"));
    cJSON_AddItemToObject(root, "sub", subarr);
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_AddItemToArray(arr, detached_meta);
    cJSON *meta_ref = cJSON_Duplicate(detached_meta, 1);
    cJSON_AddItemToArray(arr, meta_ref);
    cJSON *sub_ref = cJSON_Duplicate(subarr, 1);
    cJSON_AddItemToArray(arr, sub_ref);

    // step 3: Operate and Validate
    cJSON *newnum = cJSON_CreateNumber(3.14);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, newnum);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[128];
    buffer[0] = 0;
    int summary = cJSON_GetArraySize(arr) + (printed ? (int)printed[0] : 0) + (int)replaced + (int)buffer[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}