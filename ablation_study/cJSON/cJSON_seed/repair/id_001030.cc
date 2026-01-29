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
//<ID> 1030
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddBoolToObject(config, "feature_enabled", 1);
    cJSON_AddBoolToObject(config, "feature_flag", 0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "count", count);

    // step 3: Operate and Validate
    cJSON_bool has_enabled = cJSON_HasObjectItem(config, "feature_enabled");
    cJSON_AddItemToObject(root, "has_enabled", cJSON_CreateBool(has_enabled));
    cJSON *flag_item = cJSON_GetObjectItem(config, "feature_flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_item);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON_AddItemToObject(root, "flag_is_bool", cJSON_CreateBool(flag_is_bool));
    cJSON_AddItemToObject(root, "flag_is_false", cJSON_CreateBool(flag_is_false));
    char *updated_name = cJSON_SetValuestring(name, "updated");
    (void)updated_name;

    // step 4: Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}