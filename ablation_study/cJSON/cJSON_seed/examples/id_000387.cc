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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"tester\",\"value\":10,\"opt\":null}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *name_src = cJSON_GetObjectItem(parsed, "name");
    const char *name_str = cJSON_GetStringValue(name_src);
    cJSON *name_item = cJSON_CreateString(name_str);
    cJSON_AddItemToObjectCS(root, "name", name_item);
    cJSON *value_src = cJSON_GetObjectItem(parsed, "value");
    double new_value = cJSON_SetNumberHelper(value_src, 99.5);
    cJSON *updated_value_item = cJSON_CreateNumber(new_value);
    cJSON_AddItemToObjectCS(root, "updated_value", updated_value_item);

    // step 3: Operate & Validate
    cJSON *opt_item = cJSON_GetObjectItem(parsed, "opt");
    cJSON_bool was_null = cJSON_IsNull(opt_item);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObjectCS(root, "version", ver_item);
    char status_buf[64];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%d|%.1f", (int)was_null, new_value);
    cJSON *status_item = cJSON_CreateString(status_buf);
    cJSON_AddItemToObjectCS(root, "status", status_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObjectCS(root, "snapshot", snapshot_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}