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
//<ID> 211
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON *created_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "standalone_true", created_true);
    cJSON *enabled = cJSON_AddTrueToObject(config, "enabled");

    // step 2: Configure
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "flags", flags);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(flags, n1);
    cJSON_AddItemToArray(flags, n2);
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddStringToObject(sub, "name", "sub1");
    cJSON_AddItemToArray(flags, sub);
    cJSON *detached = cJSON_DetachItemViaPointer(flags, sub);

    // step 3: Operate and Validate
    cJSON_bool is_obj = cJSON_IsObject(detached);
    cJSON_AddItemToObject(root, "detached_sub", detached);
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *flags_ref = cJSON_GetObjectItem(cfg, "flags");
    int count = cJSON_GetArraySize(flags_ref);
    cJSON_AddNumberToObject(root, "flag_count", (double)count);
    cJSON_AddNumberToObject(root, "detached_was_object", (double)is_obj);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup and finish
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}