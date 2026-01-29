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
//<ID> 2143
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

    // step 2: Configure
    cJSON_AddStringToObject(config, "mode", "test_mode");
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "nested", "value");
    cJSON_AddItemToObject(config, "child", child);

    // step 3: Operate and Validate
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_bool list_is_array = cJSON_IsArray(list);
    cJSON *mode_item = cJSON_GetObjectItemCaseSensitive(config, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_item);
    int buflen = 32 + (int)list_is_array;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = mode_str[0];
    buffer[1] = (char)('0' + ((int)list_is_array % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}