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
//<ID> 991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");

    // step 2: Configure
    cJSON_AddNullToObject(settings, "optional");
    cJSON_AddBoolToObject(settings, "enabled", 1);
    cJSON_AddNumberToObject(settings, "timeout", 30.0);

    // step 3: Operate & Validate
    cJSON *enabled_item = cJSON_GetObjectItem(settings, "enabled");
    cJSON *enabled_cs = cJSON_GetObjectItemCaseSensitive(settings, "enabled");
    cJSON_bool enabled_consistent = (enabled_item == enabled_cs);
    cJSON *timeout_item = cJSON_GetObjectItem(settings, "timeout");
    double timeout_value = cJSON_GetNumberValue(timeout_item);
    cJSON_bool enabled_true = cJSON_IsTrue(enabled_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *raw = cJSON_PrintUnformatted(root);
    cJSON *snapshot_ref = cJSON_CreateStringReference(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot_ref);
    cJSON_free(raw);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}