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
//<ID> 764
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "enabled", true_item);

    // step 2: Configure
    cJSON_AddBoolToObject(config, "flag", 1);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *config_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObjectCS(root, "config_ref", config_ref);

    // step 3: Operate
    char json_buf[] = "{\"a\": 1, \"b\": true }";
    cJSON_Minify(json_buf);
    cJSON *min_str = cJSON_CreateString(json_buf);
    cJSON_AddItemToObject(root, "minified", min_str);
    cJSON_AddBoolToObject(root, "active", 0);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}