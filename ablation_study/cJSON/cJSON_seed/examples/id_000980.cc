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
//<ID> 980
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
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON *feature_false = cJSON_CreateFalse();
    cJSON *ver = cJSON_CreateString("1.2.3");
    cJSON *limit = cJSON_CreateNumber(100.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "enabled", enabled_true);
    cJSON_AddItemToObject(config, "version", ver);
    cJSON_AddItemToObject(config, "limit", limit);
    cJSON_AddItemToObject(config, "feature_disabled", feature_false);

    // step 3: Operate & Validate
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_feature_flag = cJSON_GetObjectItem(got_config, "feature_disabled");
    double disabled_numeric = (double)cJSON_IsFalse(got_feature_flag);
    cJSON *disabled_val = cJSON_CreateNumber(disabled_numeric);
    cJSON_AddItemToObject(root, "disabled_numeric", disabled_val);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}