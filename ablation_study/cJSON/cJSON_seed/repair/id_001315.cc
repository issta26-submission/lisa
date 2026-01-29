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
//<ID> 1315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag_on = cJSON_CreateBool(1);
    cJSON *flag_off = cJSON_CreateBool(0);
    cJSON *explicit_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "enabled", flag_on);
    cJSON_AddItemToObject(child, "disabled", flag_off);
    cJSON_AddItemToObject(child, "explicit_null", explicit_null);
    cJSON *auto_null = cJSON_AddNullToObject(root, "auto_null");

    // step 2: Configure
    cJSON *sub_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(child, "sub_enabled", sub_flag);
    cJSON *orphan_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "orphan_null", orphan_null);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON_AddItemToObject(root, "reattached_child", detached);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (cJSON_IsBool(flag_on) ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (cJSON_IsNull(auto_null) ? 1 : 0));
    ((char *)scratch)[3] = (char)('0' + (cJSON_IsBool(sub_flag) ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}