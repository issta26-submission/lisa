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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "child1");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str1);

    /* Use deep duplicates instead of reference objects/arrays to avoid
       dangling pointers / double-free issues while preserving the
       original sequence's observable structure. */
    cJSON *obj_ref = cJSON_Duplicate(child, 1);
    cJSON_AddItemToArray(arr, obj_ref);
    cJSON *arr_ref = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_ref", arr_ref);
    cJSON *detached_child = cJSON_DetachItemFromObjectCaseSensitive(root, "child");
    cJSON_AddItemToArray(arr, detached_child);

    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInArray(arr, 1, replacement);
    char *printed_unfmt = cJSON_PrintUnformatted(root);
    cJSON *got = cJSON_GetArrayItem(arr, 1);
    const char *got_str = cJSON_GetStringValue(got);
    int summary = (int)(got_str ? got_str[0] : 0) + (int)(printed_unfmt ? printed_unfmt[0] : 0) + (int)replaced_ok;
    (void)summary;
    cJSON_free(printed_unfmt);

    cJSON_Delete(root);
    return 66;
}