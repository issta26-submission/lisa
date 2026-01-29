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
//<ID> 987
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
    cJSON *name = cJSON_CreateString("device-alpha");
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *flag_true = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddItemToObject(config, "version", version);
    cJSON_AddItemToObject(root, "disabled", flag_false);
    cJSON_AddItemToObject(root, "enabled", flag_true);

    // step 3: Operate & Validate
    cJSON *got_disabled = cJSON_GetObjectItem(root, "disabled");
    cJSON_bool disabled_check = cJSON_IsFalse(got_disabled);
    cJSON *disabled_as_number = cJSON_CreateNumber((double)disabled_check);
    cJSON_AddItemToObject(root, "disabled_numeric", disabled_as_number);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}