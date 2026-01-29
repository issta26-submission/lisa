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
//<ID> 405
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *original_json = "{ \"title\": \"Example\", \"items\": [\"one\", \"two\", \"three\"] }";
    char json_buf[256];
    memset(json_buf, 0, sizeof(json_buf));
    sprintf(json_buf, "%s", original_json);
    cJSON_Minify(json_buf);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);
    cJSON *title = cJSON_GetObjectItem(root, "title");
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    const char *more_items[] = { "four", "five" };
    cJSON *extra = cJSON_CreateStringArray(more_items, 2);
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw_item = cJSON_CreateRaw("unparsed:raw_value");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate & Validate
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "raw=%d|title=%s|first=%s", (int)raw_is_raw, cJSON_GetStringValue(title), first_str ? first_str : "null");
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}