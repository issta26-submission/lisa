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
//<ID> 217
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
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");
    cJSON *separate_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "flag", separate_true);

    // step 2: Configure
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "root_is_object", (double)root_is_obj);
    cJSON *detached_flag = cJSON_DetachItemViaPointer(child, separate_true);
    cJSON_AddItemToObject(root, "moved_flag", detached_flag);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "serialized", out);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}