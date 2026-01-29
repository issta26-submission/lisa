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
//<ID> 693
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *colors[] = { "red", "green", "blue" };
    cJSON *color_array = cJSON_CreateStringArray(colors, 3);
    cJSON_AddItemToObject(root, "colors", color_array);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON_bool is_array = cJSON_IsArray(color_array);
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    int colors_count = cJSON_GetArraySize(color_array);
    cJSON *first_color = cJSON_GetArrayItem(color_array, 0);
    const char *first_color_str = cJSON_GetStringValue(first_color);
    cJSON *first_copy = cJSON_CreateString(first_color_str);
    cJSON_AddItemToObject(root, "first_color", first_copy);
    cJSON_AddBoolToObject(root, "array_is_array", is_array);
    cJSON_AddBoolToObject(root, "raw_is_raw", is_raw);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    (void)colors_count;
    (void)enabled_item;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}