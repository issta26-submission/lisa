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
//<ID> 665
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *flag1 = cJSON_CreateTrue();
    cJSON *flag2 = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "flag1", flag1);
    cJSON_AddItemToObject(settings, "flag2", flag2);

    // step 2: Configure
    cJSON *settings_copy = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_copy);

    // step 3: Operate
    cJSON *orphan1 = cJSON_DetachItemFromObjectCaseSensitive(settings, "flag2");
    cJSON *orphan2 = cJSON_DetachItemFromObject(settings_copy, "flag1");
    cJSON_AddItemToObject(root, "orphan_from_settings", orphan1);
    cJSON_AddItemToObject(root, "orphan_from_copy", orphan2);

    // step 4: Validate & Cleanup
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}