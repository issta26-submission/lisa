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
//<ID> 999
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
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddNullToObject(config, "optional");
    cJSON_AddBoolToObject(config, "featureX", 0);

    // step 3: Operate and Validate
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(got_enabled);
    cJSON_AddBoolToObject(root, "enabled_copy", is_enabled);
    cJSON *got_optional = cJSON_GetObjectItemCaseSensitive(config, "optional");
    cJSON_AddItemReferenceToObject(root, "optional_ref", got_optional);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}