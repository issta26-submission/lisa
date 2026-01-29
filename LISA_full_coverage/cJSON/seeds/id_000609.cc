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
//<ID> 609
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
    cJSON_AddNumberToObject(config, "threads", 4);
    cJSON_AddStringToObject(config, "mode", "fast");
    cJSON_AddBoolToObject(config, "enabled", 1);

    // step 2: Configure
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *threads_item = cJSON_GetObjectItem(config_ref, "threads");
    cJSON *mode_item = cJSON_GetObjectItem(config_ref, "mode");
    double threads_val = cJSON_GetNumberValue(threads_item);
    char *mode_str = cJSON_GetStringValue(mode_item);
    double increased_threads = threads_val + 2.0;

    // step 3: Operate
    cJSON *dup_config = cJSON_Duplicate(config_ref, 1);
    cJSON *dup_threads = cJSON_GetObjectItem(dup_config, "threads");
    cJSON_SetNumberHelper(dup_threads, increased_threads);
    cJSON_AddItemToObject(root, "config_backup", dup_config);

    // step 4: Validate & Cleanup
    cJSON *backup = cJSON_GetObjectItem(root, "config_backup");
    cJSON *backup_threads = cJSON_GetObjectItem(backup, "threads");
    double backup_threads_val = cJSON_GetNumberValue(backup_threads);
    (void)backup_threads_val;
    (void)mode_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}