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
//<ID> 406
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[256];
    strcpy(json_buf, " { \"name\" : \"example\" , \"values\" : [ \"x\" , \"y\" ] /*comment*/ } ");
    cJSON_Minify(json_buf);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *values_item = cJSON_GetObjectItem(root, "values");

    // step 2: Configure
    const char *new_strings[] = { "one", "two", "three" };
    cJSON *string_array = cJSON_CreateStringArray(new_strings, 3);
    cJSON_AddItemToObject(root, "new_strings", string_array);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":123}");
    cJSON_AddItemToObject(root, "raw_payload", raw_item);

    // step 3: Operate & Validate
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    const char *name_str = cJSON_GetStringValue(name_item);
    sprintf(status_buf, "name=%s|raw=%d|arr_count=%d", name_str ? name_str : "null", (int)raw_is_raw, cJSON_GetArraySize(string_array));
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