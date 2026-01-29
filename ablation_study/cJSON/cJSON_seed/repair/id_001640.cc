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
//<ID> 1640
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    char *setret = cJSON_SetValuestring(name, "updated");
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(config, "enabled", replacement_true);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItem(config, "name");
    const char *namestr = cJSON_GetStringValue(got_name);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = namestr ? namestr[0] : 'X';
    buffer[1] = (char)('0' + (int)replaced);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}