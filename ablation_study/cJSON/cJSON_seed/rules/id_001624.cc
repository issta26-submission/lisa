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
//<ID> 1624
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
    cJSON *true_item = NULL;
    cJSON *placeholder_item = NULL;
    cJSON *replacement = NULL;
    cJSON *detached = NULL;
    char *out = NULL;
    const char json_text[] = "{\"nested\":{\"num\":42}}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    parsed = cJSON_Parse(json_text);
    cJSON_AddItemToObject(root, "parsed_payload", parsed);
    true_item = cJSON_AddTrueToObject(child, "active");
    placeholder_item = cJSON_CreateString("will_be_replaced");
    cJSON_AddItemToObject(root, "placeholder", placeholder_item);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemViaPointer(root, parsed);
    cJSON_AddItemToObject(child, "migrated_payload", detached);
    replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(root, placeholder_item, replacement);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}