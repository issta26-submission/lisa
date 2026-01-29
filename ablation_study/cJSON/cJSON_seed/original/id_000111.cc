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
//<ID> 111
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num);
    cJSON *str = cJSON_CreateString("first");
    cJSON_AddItemToArray(items, str);

    // step 2: Configure
    // Use duplicates instead of reference objects to avoid dangling-reference/double-free issues.
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToArray(items, meta_copy);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_ref", items_copy);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_AddItemToObject(root, "meta_restored", detached);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInArray(items, 1, replacement);
    char *printed_unfmt = cJSON_PrintUnformatted(root);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    int items_size = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    int summary = items_size + (printed_unfmt ? (int)printed_unfmt[0] : 0) + (first_str ? (int)first_str[0] : 0) + (int)buffer[0] + (int)replaced_ok;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed_unfmt);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}