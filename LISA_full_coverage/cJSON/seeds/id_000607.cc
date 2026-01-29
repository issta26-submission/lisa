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
//<ID> 607
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
    cJSON *th_item_init = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(config, "threshold", th_item_init);
    cJSON_AddNumberToObject(config, "max", 100.0);
    cJSON_AddStringToObject(config, "mode", "auto");

    // step 2: Configure
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *dup_config = cJSON_Duplicate(config_ref, 1);
    cJSON_AddItemToObject(root, "backup_config", dup_config);

    // step 3: Operate
    cJSON *th_before = cJSON_GetObjectItem(config_ref, "threshold");
    double val_before = cJSON_GetNumberValue(th_before);
    double updated_val = val_before + 7.5;
    cJSON_ReplaceItemInObject(config_ref, "threshold", cJSON_CreateNumber(updated_val));

    // step 4: Validate & Cleanup
    cJSON *th_after = cJSON_GetObjectItem(config_ref, "threshold");
    double val_after = cJSON_GetNumberValue(th_after);
    cJSON_AddNumberToObject(root, "threshold_total", val_before + val_after);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}