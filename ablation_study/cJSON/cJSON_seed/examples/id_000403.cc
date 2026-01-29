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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { /* example comment */ \"title\" : \"example\" , \"values\" : [ 1 , 2 , 3 ] } ";
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);
    cJSON *title = cJSON_GetObjectItem(root, "title");
    cJSON *values = cJSON_GetObjectItem(root, "values");

    // step 2: Configure
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw_item = cJSON_CreateRaw("true");
    cJSON_AddItemToObject(root, "raw_bool", raw_item);

    // step 3: Operate & Validate
    cJSON_bool raw_is = cJSON_IsRaw(raw_item);
    cJSON *first_str = cJSON_GetArrayItem(str_array, 0);
    const char *first_val = cJSON_GetStringValue(first_str);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "raw=%d|first=%s|title=%s", (int)raw_is, first_val, cJSON_GetStringValue(title));
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