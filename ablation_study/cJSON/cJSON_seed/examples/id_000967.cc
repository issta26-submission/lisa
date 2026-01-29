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
//<ID> 967
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-alpha");
    cJSON *status = cJSON_CreateString("offline");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "status", status);
    cJSON_AddFalseToObject(root, "enabled");

    // step 2: Configure & Operate
    char *prev_status = cJSON_SetValuestring(status, "online");
    cJSON *mode = cJSON_CreateString("automatic");
    cJSON_AddItemToObject(root, "mode", mode);

    // step 3: Validate
    char *current_status = cJSON_GetStringValue(status);
    cJSON *status_copy = cJSON_CreateString(current_status);
    cJSON_AddItemToObject(root, "status_copy", status_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    (void)prev_status;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}