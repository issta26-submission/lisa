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
//<ID> 1626
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *true_item = NULL;
    cJSON *replacement = NULL;
    cJSON *detached = NULL;
    char *out = NULL;
    const char *json = "{\"root\":{}}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "added_child", child);
    true_item = cJSON_AddTrueToObject(child, "active");

    // step 3: Operate / Validate
    replacement = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(child, true_item, replacement);
    detached = cJSON_DetachItemViaPointer(root, child);
    cJSON_AddItemToObject(root, "restored_child", detached);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}