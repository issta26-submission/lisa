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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, num);
    cJSON *str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, str);

    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_dup", meta_dup);
    cJSON *meta_dup_copy_for_items = cJSON_Duplicate(meta_dup, 1);
    cJSON_AddItemToArray(items, meta_dup_copy_for_items);
    cJSON *items_ref_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_ref", items_ref_dup);

    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "meta_dup");
    cJSON_AddItemToArray(items, detached);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(items, 0, replacement);
    char *printed = cJSON_PrintUnformatted(root);
    int array_size = cJSON_GetArraySize(items);
    (void)array_size;

    cJSON_free(printed);
    cJSON_Delete(root);

    return 66;
}