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
//<ID> 360
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
    cJSON *title = cJSON_CreateString("palette");
    cJSON_AddItemToObject(root, "title", title);
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);

    // step 2: Configure
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool wrote = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)wrote;
    cJSON *snapshot = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 3: Operate & Validate
    cJSON_bool is_obj = cJSON_IsObject(root);
    (void)is_obj;
    cJSON_bool is_null = cJSON_IsNull(maybe_null);
    (void)is_null;
    int count = cJSON_GetArraySize(colors);
    char count_buf[32];
    sprintf(count_buf, "%d", count);
    cJSON *count_str = cJSON_CreateString(count_buf);
    cJSON_AddItemToObject(root, "count", count_str);
    cJSON *validated = cJSON_CreateString("validated");
    cJSON_AddItemToObject(root, "status", validated);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}