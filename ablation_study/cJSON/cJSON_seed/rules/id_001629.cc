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
//<ID> 1629
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
    cJSON *parsed = NULL;
    cJSON *detached = NULL;
    cJSON *true_item = NULL;
    cJSON *found_true = NULL;
    cJSON *replacement = NULL;
    char *out = NULL;
    cJSON_bool replace_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    true_item = cJSON_AddTrueToObject(child, "active");
    parsed = cJSON_Parse("{\"migrated\":false}");
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemViaPointer(root, parsed);
    cJSON_AddItemToObject(child, "migrated", detached);
    found_true = cJSON_GetObjectItem(child, "active");
    replacement = cJSON_CreateFalse();
    replace_ok = cJSON_ReplaceItemViaPointer(child, found_true, replacement);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}