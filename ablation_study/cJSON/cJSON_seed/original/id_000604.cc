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
//<ID> 604
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
    cJSON *name_item = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON *version_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(config, "version", version_item);

    // step 2: Configure
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *name_ref = cJSON_GetObjectItem(config_ref, "name");
    cJSON *ver_ref = cJSON_GetObjectItem(config_ref, "version");
    char *name_str = cJSON_GetStringValue(name_ref);
    double ver_val = cJSON_GetNumberValue(ver_ref);
    double new_version = ver_val + 2.5;
    cJSON *backup = cJSON_Duplicate(config_ref, 1);
    cJSON_AddItemToObject(root, "backup", backup);

    // step 3: Operate
    cJSON_SetNumberHelper(ver_ref, new_version);

    // step 4: Validate & Cleanup
    cJSON *backup_ref = cJSON_GetObjectItem(root, "backup");
    cJSON *backup_name = cJSON_GetObjectItem(backup_ref, "name");
    cJSON *backup_ver = cJSON_GetObjectItem(backup_ref, "version");
    char *backup_name_str = cJSON_GetStringValue(backup_name);
    double backup_ver_val = cJSON_GetNumberValue(backup_ver);
    double verification_value = ver_val + new_version + backup_ver_val;
    cJSON *check_item = cJSON_CreateNumber(verification_value);
    cJSON_AddItemToObject(root, "verification", check_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}