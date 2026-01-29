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
//<ID> 1090
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example");
    cJSON *status_old = cJSON_CreateString("old");
    cJSON *enabled_false = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "enabled", enabled_false);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "status", status_old);

    // step 3: Operate & Validate
    const char *name_val = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *status_new = cJSON_CreateString("updated");
    cJSON_ReplaceItemInObject(root, "status", status_new);
    cJSON *enabled_item = cJSON_GetObjectItem(meta, "enabled");
    cJSON *enabled_new = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(meta, enabled_item, enabled_new);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}