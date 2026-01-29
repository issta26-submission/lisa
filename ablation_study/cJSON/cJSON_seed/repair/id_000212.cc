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
//<ID> 212
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
    cJSON *t1 = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "inner_flag", t1);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(root, "name", "root_node");

    // step 2: Configure
    cJSON *t2 = cJSON_AddTrueToObject(root, "root_flag");
    (void)t2;
    cJSON *t3 = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "another_flag", t3);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON_bool detached_is_obj = cJSON_IsObject(detached);
    cJSON_AddNumberToObject(root, "detached_is_object", (double)detached_is_obj);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}