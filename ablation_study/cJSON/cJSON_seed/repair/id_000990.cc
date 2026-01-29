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
//<ID> 990
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON_AddNullToObject(config, "optional");
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON_AddNumberToObject(config, "threshold", 3.5);

    // step 3: Operate and Validate
    cJSON *cfg_cs = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *enabled_item = cJSON_GetObjectItem(cfg_cs, "enabled");
    cJSON *optional_item = cJSON_GetObjectItemCaseSensitive(cfg_cs, "optional");
    cJSON *threshold_item = cJSON_GetObjectItem(cfg_cs, "threshold");
    double thr = cJSON_GetNumberValue(threshold_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON_AddStringToObject(root, "snapshot", buffer);
    cJSON_AddNumberToObject(root, "threshold_copy", thr);
    cJSON_AddBoolToObject(root, "enabled_copy", cJSON_IsTrue(enabled_item));
    cJSON_AddNullToObject(root, "optional_copy");
    (void)printed;
    (void)optional_item;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}