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
//<ID> 996
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

    // step 2: Configure
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON_AddNullToObject(config, "optional");
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddStringToObject(root, "name", "example_project");

    // step 3: Operate and Validate
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    cJSON *optional_item = cJSON_GetObjectItemCaseSensitive(config, "optional");
    cJSON *count_item = cJSON_GetObjectItemCaseSensitive(root, "count");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_bool enabled_state = cJSON_IsTrue(enabled_item);
    cJSON_bool optional_is_null = cJSON_IsNull(optional_item);
    double count_value = cJSON_GetNumberValue(count_item);
    const char *name_value = cJSON_GetStringValue(name_item);
    (void)enabled_state;
    (void)optional_is_null;
    (void)count_value;
    (void)name_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}