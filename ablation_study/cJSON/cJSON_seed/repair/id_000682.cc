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
//<ID> 682
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
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(child, "value", num);
    cJSON *opt_null = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "optional", opt_null);

    // step 2: Configure
    double new_val = cJSON_SetNumberHelper(num, 6.28);
    cJSON_AddNumberToObject(root, "top_value", 42.0);
    cJSON_AddNumberToObject(child, "adjusted", new_val * 2.0);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(child, num);
    cJSON_AddItemToObject(root, "detached_value", detached);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}