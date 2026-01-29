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
//<ID> 962
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-1");
    cJSON *status = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON_SetValuestring(name, "device-2");
    cJSON_SetValuestring(status, "running");
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(info, "status", status);
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddFalseToObject(root, "active");

    // step 3: Operate & Validate
    const char *name_val = cJSON_GetStringValue(name);
    cJSON *copied_name = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "copied_name", copied_name);
    cJSON_SetValuestring(status, "stopped");
    cJSON *status_after = cJSON_CreateString(cJSON_GetStringValue(status));
    cJSON_AddItemToObject(info, "status_after", status_after);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}