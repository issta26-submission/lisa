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
//<ID> 1031
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
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON *feature_off_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled_item);
    cJSON_AddItemToObject(config, "feature_off", feature_off_item);
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name_item);

    // step 2: Configure
    char *updated_ptr = cJSON_SetValuestring(name_item, "updated");
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    cJSON_AddItemToObject(root, "has_config", cJSON_CreateBool(has_cfg));
    cJSON_bool is_false_flag = cJSON_IsFalse(feature_off_item);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);

    // step 3: Operate and Validate
    cJSON_AddItemToObject(root, "feature_off_detected", cJSON_CreateBool(is_false_flag));
    cJSON_AddItemToObject(root, "enabled_is_bool", cJSON_CreateBool(enabled_is_bool));
    cJSON_AddItemToObject(root, "name_copy", cJSON_CreateString(updated_ptr));
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}