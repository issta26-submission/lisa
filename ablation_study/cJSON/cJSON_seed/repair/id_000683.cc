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
//<ID> 683
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(cfg, "value", num);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "extra", 3.14);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(cfg, "maybe", nullable);

    // step 3: Operate and Validate
    cJSON_SetNumberHelper(num, 42.5);
    cJSON *detached = cJSON_DetachItemViaPointer(cfg, num);
    cJSON_AddItemToObject(root, "detached_value", detached);
    double v = cJSON_GetNumberValue(detached);
    cJSON_AddNumberToObject(root, "recomputed", v * 2.0);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}