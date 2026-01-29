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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(items, false_item);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);

    // step 3: Operate and Validate
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(items, false_item, replacement_true);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool is_bool = cJSON_IsBool(first);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}