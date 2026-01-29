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
//<ID> 1307
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
    cJSON *mode = cJSON_CreateString("fast");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(settings, "Mode", mode);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObjectCS(root, "enabled", enabled);
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 2: Configure
    cJSON *mode_item = cJSON_GetObjectItem(settings, "Mode");
    const char *mode_str = cJSON_GetStringValue(mode_item);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled);
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    size_t scratch_size = 128;
    void *scratch = cJSON_malloc(scratch_size);
    memset(scratch, 0, scratch_size);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = mode_str ? mode_str[0] : 0;
    ((char *)scratch)[2] = (char)(enabled_is_bool + enabled_is_true);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}