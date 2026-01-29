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
//<ID> 1642
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *mode_item = cJSON_AddStringToObject(settings, "mode", "active");
    cJSON *dynamic_item = cJSON_CreateString("dynamic_value");
    cJSON_AddItemToObject(settings, "dyn", dynamic_item);

    // step 2: Configure
    const char *version = cJSON_Version();
    cJSON *ver_item = cJSON_AddStringToObject(root, "library_version", version);
    cJSON *mode_copy = cJSON_CreateString(cJSON_GetStringValue(mode_item));
    cJSON_AddItemToObject(root, "mode_copy", mode_copy);

    // step 3: Operate & Validate
    cJSON *retrieved_dyn = cJSON_GetObjectItem(settings, "dyn");
    const char *retrieved_dyn_str = cJSON_GetStringValue(retrieved_dyn);
    cJSON *dyn_duplicate = cJSON_CreateString(retrieved_dyn_str);
    cJSON_AddItemToObject(root, "dyn_copy", dyn_duplicate);

    // step 4: Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    char *formatted = cJSON_Print(root);
    cJSON_free(formatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}