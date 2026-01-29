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
//<ID> 404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"title\" : \"test\", \"data\" : [ \"a\", \"b\", \"c\" ] } ";
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);
    const char *new_strings[] = { "one", "two", "three" };
    cJSON *str_array = cJSON_CreateStringArray(new_strings, 3);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON *raw_item = cJSON_CreateRaw("raw_content");

    // step 2: Configure
    cJSON_AddItemToObject(root, "new_strings", str_array);
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate & Validate
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "raw=%d|minified=%s", (int)raw_is_raw, json_buf);
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