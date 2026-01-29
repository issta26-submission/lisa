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
//<ID> 369
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
    cJSON *title = cJSON_CreateString("basic_palette");
    cJSON_AddItemToObject(root, "title", title);
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_missing", maybe_missing);

    // step 2: Configure
    char *meta_buf = (char *)cJSON_malloc(64);
    memset(meta_buf, 0, 64);
    int cnt = cJSON_GetArraySize(colors);
    sprintf(meta_buf, "count=%d", cnt);
    cJSON *meta = cJSON_CreateString(meta_buf);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_free(meta_buf);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(maybe_missing);
    cJSON_bool is_root_obj = cJSON_IsObject(root);
    char status_buf[32];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%d|%d", (int)was_null, (int)is_root_obj);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapstr);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}