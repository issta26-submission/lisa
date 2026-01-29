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
//<ID> 1059
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *settings = NULL;
    cJSON *settings_ref = NULL;
    cJSON *got_ref = NULL;
    cJSON *replacement_bool = NULL;
    char *printed = NULL;
    const char *version = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    cJSON_AddStringToObject(settings, "mode", "initial");
    version = cJSON_Version();
    cJSON_AddStringToObject(settings, "lib_version", version);
    cJSON_AddItemToObject(root, "settings", settings);
    settings_ref = cJSON_CreateObjectReference(settings);
    cJSON_AddItemToObject(root, "settings_ref", settings_ref);

    // step 3: Operate / Validate
    replacement_bool = cJSON_CreateBool(1);
    got_ref = cJSON_GetObjectItemCaseSensitive(root, "settings_ref");
    cJSON_ReplaceItemViaPointer(root, got_ref, replacement_bool);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}