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
//<ID> 923
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *root_key = "root";
    const char *settings_key = "settings";
    const char *name_key = "name";
    const char *name_val = "demo";
    const char *enabled_key = "enabled";
    cJSON *root = (cJSON *)0;
    cJSON *settings = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *enabled_item = (cJSON *)0;
    cJSON_bool has_root = (cJSON_bool)0;
    cJSON_bool has_name = (cJSON_bool)0;
    cJSON_bool has_enabled = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root and a nested settings object
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, settings_key, settings);

    // step 3: Core operations - add a string via cJSON_AddStringToObject and create a false boolean
    name_item = cJSON_AddStringToObject(settings, name_key, name_val);
    enabled_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(settings, enabled_key, enabled_item);

    // step 4: Operate - check presence of items using cJSON_HasObjectItem
    has_root = cJSON_HasObjectItem(root, settings_key);
    has_name = cJSON_HasObjectItem(settings, name_key);
    has_enabled = cJSON_HasObjectItem(settings, enabled_key);

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)has_root + (int)(settings != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)has_name + (int)has_enabled;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}