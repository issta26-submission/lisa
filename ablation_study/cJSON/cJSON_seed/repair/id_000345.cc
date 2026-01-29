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
//<ID> 345
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
    cJSON *base_num = cJSON_CreateNumber(123.5);
    cJSON_AddItemToObject(config, "base", base_num);
    cJSON *name_str = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", name_str);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", flag_true);

    // step 2: Configure
    cJSON_AddItemToObject(root, "config", config);
    cJSON *multiplier = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "multiplier", multiplier);

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name_str);
    cJSON_bool flag_val = cJSON_IsTrue(flag_true);
    double base_val = cJSON_GetNumberValue(base_num);
    double mul_val = cJSON_GetNumberValue(multiplier);
    cJSON *result = cJSON_CreateNumber(base_val * mul_val + (double)flag_val);
    cJSON_AddItemToObject(root, "result", result);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}