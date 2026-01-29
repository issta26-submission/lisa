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
//<ID> 1317
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
    cJSON *null_item = cJSON_AddNullToObject(child, "missing");
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(child, "enabled", flag);
    cJSON_AddItemToObject(root, "config", child);
    cJSON *explicit_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "explicit_null", explicit_null);

    // step 2: Configure
    cJSON *detached_flag = cJSON_DetachItemViaPointer(child, flag);
    cJSON_AddItemToObject(root, "global_enabled", detached_flag);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (cJSON_IsNull(null_item) ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (cJSON_IsTrue(detached_flag) ? 1 : 0));
    int child_count = cJSON_GetArraySize(child);
    ((char *)scratch)[3] = (char)('0' + (child_count % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}