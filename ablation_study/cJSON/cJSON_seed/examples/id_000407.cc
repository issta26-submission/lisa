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
//<ID> 407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char raw_json[] = "  { \"info\" : \" example text \", \"nums\" : [ 1, 2, 3 ] }  ";
    cJSON_Minify(raw_json);
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *root = cJSON_CreateObject();
    const char *const words[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);

    // step 2: Configure
    cJSON *raw_node = cJSON_CreateRaw("unquoted_raw_value");
    cJSON_AddItemToObject(root, "raw", raw_node);
    cJSON *info_item = cJSON_GetObjectItem(parsed, "info");
    const char *info_str = cJSON_GetStringValue(info_item);
    cJSON *info_copy = cJSON_CreateString(info_str);
    cJSON_AddItemToObject(root, "info_copy", info_copy);

    // step 3: Operate & Validate
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_node);
    cJSON *first_str = cJSON_GetArrayItem(str_array, 0);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "raw=%d|first=%s|info=%s", (int)raw_is_raw, cJSON_GetStringValue(first_str), cJSON_GetStringValue(info_copy));
    cJSON *status_item = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}