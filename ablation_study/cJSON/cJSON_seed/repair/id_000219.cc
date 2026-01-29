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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToObject(inner, "inner_enabled", t);

    // step 2: Configure
    cJSON_AddItemToObject(root, "config", inner);
    cJSON_bool flag_true = cJSON_IsTrue(flag);
    cJSON_AddBoolToObject(root, "enabled_copy", flag_true);

    // step 3: Operate
    cJSON_bool is_obj = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "is_object_flag", (double)is_obj);
    cJSON *detached = cJSON_DetachItemViaPointer(root, inner);
    cJSON_AddItemToObject(root, "config_backup", detached);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}