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
//<ID> 2083
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);

    // step 2: Configure
    cJSON *active_item = cJSON_AddBoolToObject(settings, "active", 1);
    cJSON *visible_item = cJSON_AddBoolToObject(settings, "visible", 0);
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());
    cJSON_AddItemToArray(flags, cJSON_CreateFalse());
    cJSON *detached = cJSON_DetachItemFromArray(flags, 0);
    cJSON_AddItemToObject(root, "first_flag", detached);

    // step 3: Operate and Validate
    cJSON_bool active_true = cJSON_IsTrue(active_item);
    cJSON_bool detached_true = cJSON_IsTrue(detached);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)('0' + (int)active_true);
    buffer[2] = (char)('0' + (int)detached_true);
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}