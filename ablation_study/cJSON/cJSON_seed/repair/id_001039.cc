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
//<ID> 1039
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *disabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "disabled", disabled);

    // step 2: Configure
    char *new_name = cJSON_SetValuestring(name, "updated");
    (void)new_name;
    cJSON *marker = cJSON_CreateString("marker");
    cJSON_AddItemToObject(root, "marker", marker);

    // step 3: Operate and Validate
    cJSON_bool has_disabled = cJSON_HasObjectItem(config, "disabled");
    cJSON *has_disabled_item = cJSON_CreateBool(has_disabled);
    cJSON_AddItemToObject(root, "has_disabled", has_disabled_item);
    cJSON_bool disabled_is_false = cJSON_IsFalse(disabled);
    cJSON *disabled_is_false_item = cJSON_CreateBool(disabled_is_false);
    cJSON_AddItemToObject(root, "disabled_is_false", disabled_is_false_item);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled);
    cJSON *enabled_is_bool_item = cJSON_CreateBool(enabled_is_bool);
    cJSON_AddItemToObject(root, "enabled_is_bool", enabled_is_bool_item);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}