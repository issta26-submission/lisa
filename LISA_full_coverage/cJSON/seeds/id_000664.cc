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
//<ID> 664
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *group = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "group", group);
    cJSON *alpha = cJSON_CreateTrue();
    cJSON *beta = cJSON_CreateTrue();
    cJSON_AddItemToObject(group, "alpha", alpha);
    cJSON_AddItemToObject(group, "beta", beta);

    // step 2: Operate
    cJSON *detached_alpha = cJSON_DetachItemFromObject(group, "alpha");
    cJSON *detached_beta = cJSON_DetachItemFromObjectCaseSensitive(group, "beta");
    cJSON *dup_alpha = cJSON_Duplicate((const cJSON *)detached_alpha, 1);

    // step 3: Validate
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(container, "orig_alpha", detached_alpha);
    cJSON_AddItemToObject(container, "dup_alpha", dup_alpha);
    cJSON_AddItemToObject(container, "orig_beta", detached_beta);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(container, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(container);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(container);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}