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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const colors_list[3] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *colors = cJSON_CreateStringArray(colors_list, 3);
    cJSON_AddItemToObject(root, "colors", colors);
    char *dyn_buf = (char *)cJSON_malloc(64);
    memset(dyn_buf, 0, 64);
    sprintf(dyn_buf, "allocated-%d", 42);
    cJSON *dyn_str = cJSON_CreateString(dyn_buf);
    cJSON_AddItemToObject(root, "dynamic", dyn_str);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(meta, "note", note);
    cJSON_AddNullToObject(root, "maybe");

    // step 3: Operate & Validate
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool was_null = cJSON_IsNull(maybe_item);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_item);
    int num_colors = cJSON_GetArraySize(colors);
    char count_buf[32];
    sprintf(count_buf, "%d", num_colors);
    cJSON *count_str = cJSON_CreateString(count_buf);
    cJSON_AddItemToObject(root, "count", count_str);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(dyn_buf);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}