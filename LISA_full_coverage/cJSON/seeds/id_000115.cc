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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str);
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");

    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON *meta_dup = cJSON_Duplicate(detached_meta, 1);
    cJSON_AddItemToObject(root, "meta_ref", meta_dup);
    cJSON_AddItemToArray(arr, detached_meta);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_ref", arr_dup);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 0, replacement);

    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int summary = arr_size + (printed ? (int)printed[0] : 0) + (int)buffer[0] + (int)flag->type + (int)replacement->type;
    (void)summary;
    cJSON_free(printed);

    cJSON_Delete(root);
    return 66;
}