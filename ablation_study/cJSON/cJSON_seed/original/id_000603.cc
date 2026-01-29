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
//<ID> 603
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
    cJSON *mode_item = cJSON_CreateString("auto");
    cJSON *threshold_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "mode", mode_item);
    cJSON_AddItemToObject(config, "threshold", threshold_item);

    // step 2: Configure
    cJSON *cfg_ref = cJSON_GetObjectItem(root, "config");
    cJSON *mode_ref = cJSON_GetObjectItem(cfg_ref, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_ref);
    cJSON *mode_copy = cJSON_CreateString("placeholder");
    cJSON_SetValuestring(mode_copy, mode_str);
    cJSON_AddItemToObject(root, "mode_copy", mode_copy);

    // step 3: Operate
    cJSON *config_dup = cJSON_Duplicate(cfg_ref, 1);
    cJSON_AddItemToObject(root, "config_copy", config_dup);

    // step 4: Validate & Cleanup
    cJSON *copy_ref = cJSON_GetObjectItem(root, "config_copy");
    cJSON *thresh_ref = cJSON_GetObjectItem(copy_ref, "threshold");
    double copied_value = cJSON_GetNumberValue(thresh_ref);
    cJSON_SetNumberHelper(threshold_item, copied_value + 1.0);
    (void)copied_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}