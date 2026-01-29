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
//<ID> 1036
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *mode_item = cJSON_CreateString("initial");
    cJSON_SetValuestring(mode_item, "updated");
    cJSON_AddItemToObject(settings, "mode", mode_item);
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled_item);

    // step 3: Operate and Validate
    cJSON_bool has_mode = cJSON_HasObjectItem(settings, "mode");
    cJSON_AddItemToObject(root, "has_mode", cJSON_CreateBool(has_mode));
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON_bool enabled_is_false = cJSON_IsFalse(enabled_item);
    cJSON *flags_count = cJSON_CreateNumber((double)enabled_is_bool + (double)enabled_is_false);
    cJSON_AddItemToObject(root, "bool_flags_count", flags_count);

    // step 4: Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}