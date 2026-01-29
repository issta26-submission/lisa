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
//<ID> 1313
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
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *n = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "enabled", flag);
    cJSON_AddItemToObject(config, "unset", n);
    cJSON_AddNullToObject(config, "direct_null");
    cJSON_AddItemToObject(root, "config", config);
    cJSON *temp = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "temp_flag", temp);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(root, temp);
    cJSON_AddItemToObject(config, "moved_flag", detached);
    cJSON *extra_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "extra", extra_null);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *moved = cJSON_GetObjectItem(config, "moved_flag");
    cJSON_bool moved_is_bool = cJSON_IsBool(moved);
    cJSON_bool moved_is_true = cJSON_IsTrue(moved);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + moved_is_bool);
    ((char *)scratch)[2] = (char)('0' + moved_is_true);
    ((char *)scratch)[3] = (char)('0' + cJSON_IsNull(cJSON_GetObjectItem(config, "unset")));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}