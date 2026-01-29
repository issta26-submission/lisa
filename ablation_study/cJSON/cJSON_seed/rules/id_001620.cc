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
//<ID> 1620
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *active_item = NULL;
    cJSON *name_item = NULL;
    cJSON *detached_item = NULL;
    cJSON *replacement = NULL;
    char *out = NULL;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed = cJSON_Parse("{\"name\":\"alice\",\"active\":true,\"score\":42}");
    cJSON_AddItemToObject(root, "payload", parsed);
    replacement = cJSON_CreateFalse();
    active_item = cJSON_GetObjectItem(parsed, "active");
    replaced = cJSON_ReplaceItemViaPointer(parsed, active_item, replacement);
    cJSON_AddTrueToObject(parsed, "validated");

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(parsed, "name");
    detached_item = cJSON_DetachItemViaPointer(parsed, name_item);
    cJSON_AddItemToObject(root, "username", detached_item);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}