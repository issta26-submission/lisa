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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON *value_item = cJSON_CreateNumber(123.456);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "value", value_item);
    cJSON_AddItemToObject(root, "active", flag_item);
    cJSON *desc_item = cJSON_CreateString("nested description");
    cJSON_AddItemToObject(meta, "description", desc_item);

    // step 2: Configure
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    const char *name_val = cJSON_GetStringValue(name_item);
    sprintf(status_buf, "name=%s|value=%.3f", name_val ? name_val : "null", cJSON_GetNumberValue(value_item));
    cJSON *status_item = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}