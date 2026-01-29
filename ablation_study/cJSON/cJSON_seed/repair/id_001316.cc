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
//<ID> 1316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *missing = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "flag", flag);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "missing", missing);
    cJSON *explicit_null = cJSON_AddNullToObject(root, "explicit_null");

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(root, missing);
    cJSON_AddItemToObject(meta, "recovered", detached);
    cJSON *second_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "second_flag", second_flag);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = explicit_null ? (explicit_null->type ? 'N' : 'n') : 'x';
    ((char *)scratch)[2] = (char)('0' + (flag != NULL));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}