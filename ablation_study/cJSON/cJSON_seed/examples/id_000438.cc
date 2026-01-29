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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "metadata");
    cJSON *name_item = cJSON_CreateString("fuzz-example");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString(cJSON_Version()));
    cJSON_AddItemToObject(meta, "limit", cJSON_CreateNumber(100.0));

    // step 2: Configure
    cJSON *inner = cJSON_AddObjectToObject(meta, "inner");
    cJSON_AddItemToObject(inner, "enabled", cJSON_CreateBool(1));
    cJSON_AddItemToObject(inner, "note", cJSON_CreateString("nested-note"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s|snapshot=%s", name_str ? name_str : "null", snapshot ? snapshot : "null");
    cJSON_AddStringToObject(root, "status", status_buf);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}