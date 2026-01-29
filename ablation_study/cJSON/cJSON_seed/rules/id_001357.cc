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
//<ID> 1357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *num_item = NULL;
    cJSON *bool_item = NULL;
    const char *version_str = NULL;
    double num_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "initial_value", 7.25);
    num_item = cJSON_GetObjectItem(root, "initial_value");
    bool_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", bool_item);
    nested = cJSON_AddObjectToObject(root, "child_object");
    cJSON_AddNumberToObject(nested, "child_placeholder", 0.0);

    // step 3: Operate / Validate
    num_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(nested, "child_from_parent", num_val * 2.0);
    version_str = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", version_str);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}