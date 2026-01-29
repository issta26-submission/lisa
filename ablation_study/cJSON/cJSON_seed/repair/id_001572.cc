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
//<ID> 1572
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
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "pi", pi);
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    cJSON *cfg_item = cJSON_GetObjectItem(root, "config");
    cJSON_bool cfg_is_obj = cJSON_IsObject(cfg_item);
    cJSON *pi_item = cJSON_GetObjectItem(cfg_item, "pi");
    cJSON *enabled_item = cJSON_GetObjectItem(cfg_item, "enabled");
    double pi_val = cJSON_GetNumberValue(pi_item);
    double doubled = pi_val * 2.0;

    // step 3: Operate and Validate
    cJSON *double_item = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(root, "double_pi", double_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = (char)('0' + (int)cfg_is_obj);
    scratch[1] = (char)('0' + (int)cJSON_IsObject(cJSON_GetObjectItem(root, "double_pi")));
    scratch[2] = (char)('0' + (int)((int)doubled % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}