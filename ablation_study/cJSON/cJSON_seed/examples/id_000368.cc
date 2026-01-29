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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const words[3] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *str_arr = cJSON_CreateStringArray(words, 3);
    cJSON *title = cJSON_CreateString("palette");
    cJSON_AddItemToObject(root, "colors", str_arr);
    cJSON_AddItemToObject(root, "title", title);

    // step 2: Configure
    char *buffer = (char*)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 512, 0);
    (void)printed;
    cJSON *snapshot = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 3: Operate & Validate
    cJSON *colors = cJSON_GetObjectItem(root, "colors");
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool colors_is_null = cJSON_IsNull(colors);
    double flag_value = (double)root_is_obj + (double)colors_is_null;
    cJSON *flags = cJSON_CreateNumber(flag_value);
    cJSON_AddItemToObject(root, "flags", flags);
    const char *title_text = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title"));
    cJSON *title_copy = cJSON_CreateString(title_text);
    cJSON_AddItemToObject(root, "title_copy", title_copy);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}