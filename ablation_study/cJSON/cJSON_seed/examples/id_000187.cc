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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(list, item_a);
    cJSON *item_b_ref = cJSON_CreateStringReference("beta");
    cJSON_AddItemToArray(list, item_b_ref);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_child = cJSON_CreateString("child_value");
    cJSON_AddItemToObject(meta, "child", meta_child);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("gamma");
    cJSON_ReplaceItemInArray(list, 1, replacement);
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    char *printed_root = cJSON_PrintUnformatted(root);
    char *printed_meta = cJSON_PrintUnformatted(detached_meta);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_meta);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}