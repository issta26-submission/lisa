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
//<ID> 1005
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "initial");

    // step 2: Configure
    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_SetValuestring(name, "updated");
    cJSON_AddStringToObject(root, "status", cJSON_GetStringValue(name));

    // step 3: Operate and Validate
    char *serialized = cJSON_Print(root);
    cJSON *status = cJSON_GetObjectItemCaseSensitive(root, "status");
    char *status_str = cJSON_GetStringValue(status);
    cJSON_AddStringToObject(root, "status_copy", status_str);

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}