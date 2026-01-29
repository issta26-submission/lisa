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
//<ID> 214
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
    cJSON *flag = cJSON_AddTrueToObject(child, "flag");

    // step 2: Configure
    cJSON *detached_flag = cJSON_DetachItemViaPointer(child, flag);
    cJSON *true2 = cJSON_CreateTrue();

    // step 3: Operate and Validate
    cJSON_AddItemToObject(root, "detached_flag", detached_flag);
    cJSON_AddItemToObject(root, "true2", true2);
    cJSON_bool child_is_obj = cJSON_IsObject(child);
    cJSON_AddNumberToObject(root, "child_is_object", (double)child_is_obj);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}