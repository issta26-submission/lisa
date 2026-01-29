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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"widget\",\"value\":10,\"optional\":null}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON *optional_item = cJSON_GetObjectItem(root, "optional");
    cJSON *version_item = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObjectCS(root, "lib_version", version_item);

    // step 2: Configure
    double old_value = cJSON_GetNumberValue(value_item);
    double new_value = cJSON_SetNumberHelper(value_item, old_value * 2.5);
    cJSON *ratio_item = cJSON_CreateNumber(new_value / old_value);
    cJSON_AddItemToObjectCS(root, "ratio", ratio_item);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(optional_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "null=%d|old=%.2f|new=%.2f|ver=%s", (int)was_null, old_value, new_value, cJSON_GetStringValue(version_item));
    cJSON *status_item = cJSON_CreateString(status_buf);
    cJSON_AddItemToObjectCS(root, "status", status_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObjectCS(root, "snapshot", snapshot_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}