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
//<ID> 1649
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
    cJSON *manual_str = cJSON_CreateString("manual_value");
    cJSON_AddItemToObject(settings, "manual_key", manual_str);
    cJSON *auto_added = cJSON_AddStringToObject(settings, "auto_key", "auto_value");

    // step 2: Configure
    cJSON *metadata = cJSON_AddObjectToObject(root, "metadata");
    cJSON *meta_name = cJSON_AddStringToObject(metadata, "name", "example_config");
    (void)meta_name; // keep clear that meta_name is intentionally unused beyond creation

    // step 3: Operate & Validate
    cJSON *auto_item = cJSON_GetObjectItem(settings, "auto_key");
    const char *auto_val = cJSON_GetStringValue(auto_item);
    cJSON *auto_copy = cJSON_CreateString(auto_val);
    cJSON_AddItemToObject(root, "auto_copy", auto_copy);

    // step 4: Cleanup
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(unfmt);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}