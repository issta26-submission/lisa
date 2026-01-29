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
//<ID> 1621
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
    cJSON *detached = NULL;
    cJSON *replacement = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    parsed = cJSON_Parse("{\"parsed_key\":42}");
    cJSON_AddItemToObject(child, "parsed", parsed);
    true_item = cJSON_AddTrueToObject(child, "active");

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("replaced_string");
    cJSON_ReplaceItemViaPointer(child, parsed, replacement);
    detached = cJSON_DetachItemViaPointer(child, true_item);
    cJSON_AddItemToObject(root, "detached_flag", detached);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}