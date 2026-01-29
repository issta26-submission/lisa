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
//<ID> 734
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
    cJSON *threshold = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(settings, "threshold", threshold);

    // step 2: Configure
    cJSON *threshold_backup = cJSON_Duplicate(threshold, 0);
    cJSON_AddItemToObject(root, "threshold_backup", threshold_backup);

    // step 3: Operate and Validate
    cJSON *settings_ref = cJSON_GetObjectItem(root, "settings");
    cJSON *threshold_ref = cJSON_GetObjectItem(settings_ref, "threshold");
    cJSON *new_threshold = cJSON_CreateNumber(6.28318);
    cJSON_ReplaceItemViaPointer(settings_ref, threshold_ref, new_threshold);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}