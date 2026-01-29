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
//<ID> 1869
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
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *version = cJSON_CreateString("1.0");
    cJSON_AddItemToObject(config, "version", version);

    // step 2: Configure
    cJSON *ver_item = cJSON_GetObjectItem(config, "version");
    char *set_ret = cJSON_SetValuestring(ver_item, "2.0");
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    int is_enabled = (int)cJSON_IsTrue(enabled_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = set_ret ? set_ret[0] : 'V';
    buffer[2] = (char)('0' + (is_enabled % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}