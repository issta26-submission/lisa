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
//<ID> 1628
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
    cJSON *old_true = NULL;
    cJSON *new_item = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    parsed = cJSON_Parse("{\"inner\":42}");
    cJSON_AddItemToObject(root, "parsed", parsed);
    true_item = cJSON_AddTrueToObject(child, "initial_true");

    // step 3: Operate / Validate
    detached = cJSON_DetachItemViaPointer(root, parsed);
    cJSON_AddItemToObject(child, "moved", detached);
    old_true = cJSON_GetObjectItem(child, "initial_true");
    new_item = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(child, old_true, new_item);
    out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}