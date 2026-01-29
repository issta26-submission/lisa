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
//<ID> 1052
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
    cJSON *old_flag = NULL;
    cJSON *repl_flag = NULL;
    char *printed = NULL;
    const char *vers = NULL;
    cJSON_bool was_enabled = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    cJSON_AddItemToObject(settings, "enabled", cJSON_CreateBool(0));
    cJSON_AddItemToObject(root, "settings", settings);
    settings_ref = cJSON_Duplicate(settings, 1); /* use duplicate to avoid double-free from a reference */
    cJSON_AddItemToObject(root, "settings_ref", settings_ref);
    vers = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", vers);

    // step 3: Operate / Validate
    old_flag = cJSON_GetObjectItemCaseSensitive(settings, "enabled");
    repl_flag = cJSON_CreateBool(1);
    cJSON_ReplaceItemViaPointer(settings, old_flag, repl_flag);
    was_enabled = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(settings, "enabled"));
    (void)was_enabled;
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}