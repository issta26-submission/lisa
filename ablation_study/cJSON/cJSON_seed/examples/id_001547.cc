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
//<ID> 1547
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
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    (void)enabled_item;

    // step 2: Configure
    char *printed = cJSON_Print(root);
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_bool has_missing = cJSON_HasObjectItem(root, "missing");
    (void)has_config; (void)has_missing;

    // step 3: Operate & Validate
    cJSON *config_lookup = cJSON_GetObjectItem(root, "config");
    cJSON *retrieved_name = cJSON_GetObjectItem(config_lookup, "name");
    char *name_value = cJSON_GetStringValue(retrieved_name);
    (void)name_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}