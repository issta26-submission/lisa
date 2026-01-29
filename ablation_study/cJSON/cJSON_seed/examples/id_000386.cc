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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"alpha\",\"value\":3.14,\"optional\":null}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");

    // step 2: Configure
    double updated_value = cJSON_SetNumberHelper(value_item, 42.0);
    cJSON *ver_item = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObjectCS(root, "version", ver_item);
    cJSON *opt_item = cJSON_GetObjectItem(root, "optional");
    cJSON_bool was_null = cJSON_IsNull(opt_item);

    // step 3: Operate & Validate
    char status_buf[64];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "was_null=%d|value=%g", (int)was_null, updated_value);
    cJSON *status_item = cJSON_CreateString(status_buf);
    cJSON_AddItemToObjectCS(root, "status", status_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snap_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObjectCS(root, "snapshot", snap_item);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}