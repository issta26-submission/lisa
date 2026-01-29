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
//<ID> 681
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
    cJSON *pi = cJSON_CreateNumber(3.14159265);
    cJSON_AddItemToObject(child, "pi", pi);
    cJSON *nil = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "nil", nil);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 2.0);
    double updated_pi = cJSON_SetNumberHelper(pi, 6.28318530);
    cJSON_AddNumberToObject(root, "updated_pi_copy", updated_pi);

    // step 3: Operate and Validate
    cJSON *detached_pi = cJSON_DetachItemViaPointer(child, pi);
    cJSON_AddItemToObject(root, "detached_pi", detached_pi);
    double detached_value = cJSON_GetNumberValue(detached_pi);
    cJSON_AddNumberToObject(root, "detached_value_copy", detached_value);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}