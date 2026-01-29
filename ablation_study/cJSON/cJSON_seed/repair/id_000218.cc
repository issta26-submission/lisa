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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *primary = cJSON_AddTrueToObject(root, "primary");
    cJSON *extra_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, extra_true);

    // step 2: Operate
    cJSON *detached_primary = cJSON_DetachItemViaPointer(root, primary);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "root_is_object", (double)root_is_object);
    int flags_count = cJSON_GetArraySize(flags);
    cJSON_AddNumberToObject(root, "flags_count", (double)flags_count);

    // step 3: Validate
    double detached_is_true = (double)cJSON_IsTrue(detached_primary);
    cJSON_AddNumberToObject(root, "detached_primary_is_true", detached_is_true);
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_detached = cJSON_PrintUnformatted(detached_primary);

    // step 4: Cleanup
    cJSON_free(out_root);
    cJSON_free(out_detached);
    cJSON_Delete(detached_primary);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}