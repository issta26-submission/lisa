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
//<ID> 400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"user\" : \"alice\" , \"count\" : 3 } ";
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);
    const char *colors_arr[] = { "red", "green", "blue" };
    cJSON *colors = cJSON_CreateStringArray(colors_arr, 3);
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "rawdata", raw);

    // step 2: Configure
    cJSON *colors_item = cJSON_GetObjectItem(root, "colors");
    cJSON *first_color = cJSON_GetArrayItem(colors_item, 0);
    const char *first_color_val = cJSON_GetStringValue(first_color);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw);

    // step 3: Operate & Validate
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "first=%s|raw=%d", first_color_val, (int)raw_is_raw);
    cJSON *status_item = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}