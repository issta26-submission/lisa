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
//<ID> 41
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
    cJSON *active = cJSON_CreateTrue();
    cJSON *name = cJSON_CreateString("fuzz-test");
    cJSON_AddItemToObject(config, "active", active);
    cJSON_AddItemToObject(config, "name", name);

    // step 2: Configure
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *inner = cJSON_CreateString("inner-value");
    cJSON_AddItemToObject(nested, "inner_key", inner);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(nested, "flag", flag);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_active = cJSON_GetObjectItem(got_config, "active");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    cJSON_bool active_true = cJSON_IsTrue(got_active);
    const char *name_val = cJSON_GetStringValue(got_name);
    (void)active_true;
    (void)name_val;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}