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
//<ID> 1310
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "nullable", nullable);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *direct_null = cJSON_AddNullToObject(root, "direct_null");

    // step 2: Configure
    cJSON *subflag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(child, "subflag", subflag);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(child, nullable);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + (cJSON_IsTrue(flag) ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (cJSON_IsFalse(subflag) ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}