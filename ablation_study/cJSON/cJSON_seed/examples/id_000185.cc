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
//<ID> 185
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *sref = cJSON_CreateStringReference("initial_string");
    cJSON_AddItemToArray(items, sref);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num);
    cJSON *replacement = cJSON_CreateString("replaced_string");
    cJSON_ReplaceItemInArray(items, 0, replacement);

    // step 3: Operate & Validate
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_val = cJSON_GetStringValue(first_item);
    cJSON *ref_to_first = cJSON_CreateStringReference(first_val);
    cJSON_AddItemToObject(root, "first_ref", ref_to_first);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}