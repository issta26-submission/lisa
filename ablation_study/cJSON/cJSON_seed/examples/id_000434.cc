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
//<ID> 434
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON *val_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "value", val_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *meta_note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(meta, "note", meta_note);
    cJSON_AddNumberToObject(meta, "count", 7.0);

    // step 3: Operate & Validate
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(child, "name"));
    double value_num = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "value"));
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s|value=%.0f|meta_count=%.0f", name_str ? name_str : "null", value_num, cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "count")));
    cJSON_AddStringToObject(root, "status", status_buf);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}