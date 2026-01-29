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
//<ID> 1646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *status = cJSON_CreateString("initial");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "status", status);
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    char *set_result = cJSON_SetValuestring(status, "updated");
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "enabled", replacement_true);

    // step 3: Operate and Validate
    cJSON *got_status = cJSON_GetObjectItem(root, "status");
    const char *status_str = cJSON_GetStringValue(got_status);
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(got_enabled);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[16];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = status_str ? status_str[0] : 'X';
    buffer[1] = set_result ? set_result[0] : 'Y';
    buffer[2] = (char)('0' + (int)replaced);
    buffer[3] = (char)('0' + (int)is_enabled);
    buffer[4] = printed ? printed[0] : 'Z';
    (void)buffer[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}