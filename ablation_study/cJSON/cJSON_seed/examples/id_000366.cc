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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const colors[3] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *palette = cJSON_CreateArray();
    cJSON *color_array = cJSON_CreateStringArray(colors, 3);
    cJSON_AddItemToArray(palette, color_array);
    cJSON_AddItemToObject(root, "palette", palette);
    cJSON *title = cJSON_CreateString("primary");
    cJSON_AddItemToObject(root, "title", title);

    // step 2: Configure
    void *tmp_buf = cJSON_malloc(8);
    memset(tmp_buf, 0, 8);
    cJSON *empty_str = cJSON_CreateString((const char *)tmp_buf);
    cJSON_AddItemToObject(root, "empty", empty_str);
    cJSON_free(tmp_buf);
    cJSON_AddNullToObject(root, "maybe");

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(color_array);
    char count_buf[32];
    sprintf(count_buf, "%d", count);
    cJSON *count_item = cJSON_CreateString(count_buf);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool was_null = cJSON_IsNull(maybe_item);
    cJSON_bool is_object = cJSON_IsObject(root);
    (void)was_null;
    (void)is_object;

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}