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
//<ID> 1035
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
    cJSON *flag_on = cJSON_CreateTrue();
    cJSON *flag_off = cJSON_CreateFalse();
    cJSON_AddItemToObject(settings, "enabled", flag_on);
    cJSON_AddItemToObject(settings, "disabled", flag_off);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(settings, "name", name);
    char *new_name = cJSON_SetValuestring(name, "configured");
    (void)new_name;

    // step 3: Operate and Validate
    cJSON_bool has_enabled = cJSON_HasObjectItem(settings, "enabled");
    cJSON *has_item = cJSON_CreateBool(has_enabled);
    cJSON_AddItemToObject(root, "has_enabled", has_item);
    cJSON *disabled_item = cJSON_GetObjectItem(settings, "disabled");
    cJSON_bool disabled_is_bool = cJSON_IsBool(disabled_item);
    cJSON_bool disabled_is_false = cJSON_IsFalse(disabled_item);
    double summary_val = (double)disabled_is_bool + (double)disabled_is_false;
    cJSON *summary_num = cJSON_CreateNumber(summary_val);
    cJSON_AddItemToObject(root, "disabled_summary", summary_num);

    // step 4: Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}