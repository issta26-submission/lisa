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
//<ID> 1647
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

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", true_item);
    cJSON *replacement = cJSON_CreateString("updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(config, "name", replacement);
    char *after_set = cJSON_SetValuestring(replacement, "final");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const char *namestr = cJSON_GetStringValue(replacement);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = namestr ? namestr[0] : 'X';
    buffer[1] = printed ? printed[0] : 'Y';
    buffer[2] = (char)('0' + (int)replaced);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)after_set;
    return 66;
    // API sequence test completed successfully
}