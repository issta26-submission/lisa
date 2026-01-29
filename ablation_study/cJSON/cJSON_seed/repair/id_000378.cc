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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *name = cJSON_CreateString("device-01");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *opt = cJSON_AddNullToObject(cfg, "optional");

    // step 2: Configure
    const char *name_val = cJSON_GetStringValue(name);
    cJSON_bool opt_is_null = cJSON_IsNull(opt);
    double opt_flag = (double)opt_is_null;
    cJSON_AddNumberToObject(cfg, "optional_flag", opt_flag);

    // step 3: Operate and Validate
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}