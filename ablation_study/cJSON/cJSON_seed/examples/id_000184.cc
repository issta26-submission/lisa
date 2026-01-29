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
//<ID> 184
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *first = cJSON_CreateString("first_item");
    cJSON_AddItemToArray(items, first);

    // step 2: Configure
    cJSON *ref = cJSON_CreateStringReference("reference_value");
    cJSON_AddItemToObject(root, "refkey", ref);
    cJSON *replacement = cJSON_CreateStringReference("replacement_value");
    cJSON_ReplaceItemInArray(items, 0, replacement);

    // step 3: Operate & Validate
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    char *printed = cJSON_PrintUnformatted(root);
    int count = cJSON_GetArraySize(items);
    cJSON *got_ref = cJSON_GetObjectItem(root, "refkey");
    const char *got_ref_str = cJSON_GetStringValue(got_ref);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached_meta);

    // API sequence test completed successfully
    return 66;
}