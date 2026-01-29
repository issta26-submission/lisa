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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"name\" : \"widget\", \"value\" : 10, \"opt\": null } ";
    cJSON_Minify(json_buf);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value = cJSON_GetNumberValue(value_item);
    cJSON *opt_item = cJSON_GetObjectItem(root, "opt");

    // step 2: Configure
    const char *tags[] = { "alpha", "beta", "gamma" };
    cJSON *tags_arr = cJSON_CreateStringArray(tags, 3);
    cJSON_AddItemToObject(root, "tags", tags_arr);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw_blob = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root, "raw_blob", raw_blob);

    // step 3: Operate & Validate
    cJSON_bool raw_check = cJSON_IsRaw(raw_blob);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s|value=%.0f|is_raw=%d|opt_is_null=%d",
            cJSON_GetStringValue(name_item),
            value,
            (int)raw_check,
            (int)cJSON_IsNull(opt_item));
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