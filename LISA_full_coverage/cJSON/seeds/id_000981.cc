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
//<ID> 981
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"settings\": {\"mode\":\"default\"}, \"flags\": {\"enabled\":false}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *settings = cJSON_GetObjectItem(root, "settings");
    cJSON *flags = cJSON_GetObjectItem(root, "flags");
    cJSON *advanced = cJSON_AddObjectToObject(settings, "advanced");
    cJSON *enabled_item = cJSON_GetObjectItem(flags, "enabled");
    cJSON_bool was_false = cJSON_IsFalse(enabled_item);
    cJSON_AddBoolToObject(advanced, "was_enabled_false", was_false);
    cJSON *mode_item = cJSON_GetObjectItem(settings, "mode");
    char *old_mode = cJSON_SetValuestring(mode_item, "modified");

    // step 3: Operate
    cJSON_bool advanced_is_obj = cJSON_IsObject(advanced);
    char *printed = cJSON_Print(root);
    int buf_size = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_size);
    memset(buffer, 0, (size_t)buf_size);
    cJSON_PrintPreallocated(root, buffer, buf_size, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(settings, "advanced");
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}