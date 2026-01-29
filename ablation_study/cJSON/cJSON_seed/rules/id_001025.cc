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
//<ID> 1025
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
    cJSON *flag_true = NULL;
    cJSON *mode_str = NULL;
    cJSON *array = NULL;
    cJSON *detached = NULL;
    cJSON *dup_settings = NULL;
    cJSON *obj_in_array = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;
    cJSON_bool enabled_val = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", flag_true);
    mode_str = cJSON_CreateString("auto");
    cJSON_AddItemToObject(settings, "mode", mode_str);
    array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", array);

    // step 3: Operate / Validate
    dup_settings = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", dup_settings);
    detached = cJSON_DetachItemFromObject(root, "settings");
    cJSON_AddItemToArray(array, detached);
    obj_in_array = cJSON_GetArrayItem(array, 0);
    got_flag = cJSON_GetObjectItem(obj_in_array, "enabled");
    enabled_val = cJSON_IsTrue(got_flag);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)enabled_val;
    return 66;
}