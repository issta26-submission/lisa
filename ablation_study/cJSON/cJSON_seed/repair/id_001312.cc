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
//<ID> 1312
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
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *explicit_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "flag", flag);
    cJSON *maybe_null = cJSON_AddNullToObject(root, "maybe_null");

    // step 2: Configure
    cJSON *detached_null = cJSON_DetachItemViaPointer(root, maybe_null);
    cJSON_AddItemToObject(child, "readded_null", detached_null);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (cJSON_IsTrue(flag) ? 1 : 0));
    ((char *)scratch)[2] = ((char *)scratch)[0] ? ((char *)scratch)[0] : ((char *)scratch)[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}