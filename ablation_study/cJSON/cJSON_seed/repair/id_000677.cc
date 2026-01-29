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
//<ID> 677
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
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metadata", metadata);

    // step 2: Configure
    cJSON_AddStringToObject(config, "name", "example_project");
    cJSON_AddNumberToObject(config, "version", 1.23);
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "subconfig", sub);
    cJSON_AddStringToObject(sub, "detail", "subconfig detail");

    // step 3: Operate and Validate
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON_AddBoolToObject(root, "name_is_string", name_is_string);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}