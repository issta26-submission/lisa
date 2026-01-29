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
//<ID> 409
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "  { \"title\" : \"Example Document\" , \"count\" : 3, \"note\" : \" to be minified \" }  ";
    char json_buf[256];
    memset(json_buf, 0, sizeof(json_buf));
    memcpy(json_buf, raw_json, strlen(raw_json) + 1);
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);

    // step 2: Configure
    const char *tags[] = { "alpha", "beta", "gamma" };
    cJSON *tags_arr = cJSON_CreateStringArray(tags, 3);
    cJSON_AddItemToObject(root, "tags", tags_arr);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw_item = cJSON_CreateRaw("42");
    cJSON_AddItemToObject(root, "raw_value", raw_item);

    // step 3: Operate & Validate
    cJSON *title_item = cJSON_GetObjectItem(root, "title");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    double count_val = cJSON_GetNumberValue(count_item);
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "raw=%d|count=%.0f|title=%s", (int)raw_is_raw, count_val, cJSON_GetStringValue(title_item));
    cJSON *status_str = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status_str);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}