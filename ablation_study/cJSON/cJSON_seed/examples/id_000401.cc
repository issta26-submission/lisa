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
//<ID> 401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"title\" : \"example\" , \"count\" : 3 , \"items\" : [\"a\" , \"b\"] } ";
    cJSON_Minify(json_buf);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);
    cJSON *title = cJSON_GetObjectItem(root, "title");
    cJSON *count = cJSON_GetObjectItem(root, "count");
    cJSON *items = cJSON_GetObjectItem(root, "items");
    const char *colors_arr[] = { "red", "green", "blue" };
    cJSON *colors = cJSON_CreateStringArray(colors_arr, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw_item = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root, "rawdata", raw_item);

    // step 3: Operate & Validate
    int items_count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "minified=%s|items=%d|first=%s|raw=%d", json_buf, items_count, first_str ? first_str : "null", (int)raw_is_raw);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}