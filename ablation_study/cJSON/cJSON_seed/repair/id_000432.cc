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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\": {\"enabled\": true, \"threshold\": 3.5}, \"data\": {\"x\": 1}}";
    cJSON *root = cJSON_Parse(json);
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_AddBoolToObject(root, "config_present", has_config);

    // step 2: Configure
    cJSON *config_item = cJSON_GetObjectItem(root, "config");
    cJSON *added = cJSON_AddObjectToObject(root, "added");
    cJSON *nested = cJSON_AddObjectToObject(added, "nested");

    // step 3: Operate
    cJSON *config_ref = cJSON_CreateObjectReference(config_item);
    cJSON_AddItemToObject(nested, "ref", config_ref);
    char *printed = cJSON_Print(root);
    cJSON_free((void *)printed);

    // step 4: Validate and Cleanup
    cJSON_bool nested_present = cJSON_HasObjectItem(added, "nested");
    cJSON_AddBoolToObject(root, "nested_present", nested_present);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}