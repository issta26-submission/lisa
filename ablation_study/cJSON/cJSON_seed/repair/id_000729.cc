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
//<ID> 729
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
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, f);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    char *updated_name = cJSON_SetValuestring(name, "updated");

    // step 3: Operate and Validate
    cJSON *meta_ref = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *name_ref = cJSON_GetObjectItemCaseSensitive(meta_ref, "name");
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    int total_items = cJSON_GetArraySize(items_ref);
    cJSON_AddNumberToObject(root, "count", (double)total_items);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}