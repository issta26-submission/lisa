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
//<ID> 1319
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
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "nullable", nullable);
    cJSON *explicit_null = cJSON_AddNullToObject(root, "explicit_null");
    cJSON *enabled_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(config, "enabled", enabled_flag);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(config, nullable);
    cJSON_AddItemToObject(root, "moved_nullable", detached);

    // step 3: Operate and Validate
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *moved = cJSON_GetObjectItem(root, "moved_nullable");
    cJSON *exp = cJSON_GetObjectItem(root, "explicit_null");
    cJSON *enabled_ptr = cJSON_GetObjectItem(cfg, "enabled");
    cJSON_bool moved_is_null = cJSON_IsNull(moved);
    cJSON_bool exp_is_null = cJSON_IsNull(exp);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_ptr);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (moved_is_null ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (exp_is_null ? 1 : 0));
    ((char *)scratch)[3] = (char)('0' + (enabled_is_bool ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}