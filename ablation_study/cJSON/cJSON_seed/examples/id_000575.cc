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
//<ID> 575
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    const char *colors[] = { "red", "green", "blue" };
    cJSON *color_array = cJSON_CreateStringArray(colors, 3);
    cJSON_AddItemToObject(root, "colors", color_array);
    cJSON *append_array = cJSON_CreateArray();
    cJSON_AddItemToArray(append_array, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(append_array, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "append", append_array);

    // step 2: Configure
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddFalseToObject(flags, "enabled");
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_colors = cJSON_GetObjectItem(parsed, "colors");
    cJSON *first_color = cJSON_GetArrayItem(parsed_colors, 0);
    const char *first_color_str = cJSON_GetStringValue(first_color);
    cJSON *parsed_flags = cJSON_GetObjectItem(parsed, "flags");
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed_flags, "enabled");
    cJSON_bool enabled_is_false = cJSON_IsFalse(parsed_enabled);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;color=%s;enabled_false=%d",
            version ? version : "null",
            first_color_str ? first_color_str : "null",
            (int)enabled_is_false);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}