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
//<ID> 1109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_flag = NULL;
    cJSON *new_flag = NULL;
    cJSON *settings = NULL;
    cJSON *mode = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "active", old_flag);
    settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    mode = cJSON_CreateString("initial");
    cJSON_AddItemToObject(settings, "mode", mode);
    new_flag = cJSON_CreateBool(1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "active", new_flag);
    detached = cJSON_DetachItemFromObject(settings, "mode");
    cJSON_AddItemToObject(root, "mode_backup", detached);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}