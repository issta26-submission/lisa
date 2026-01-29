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
//<ID> 435
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_name = "alpha";
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *name_item = cJSON_CreateString(initial_name);
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON *id_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "id", id_item);

    // step 2: Configure
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "nested", nested);
    cJSON_AddItemToObject(nested, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(nested, "mode", cJSON_CreateString("full"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);
    char *retrieved = cJSON_GetStringValue(snapshot_item);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "snapshot_preview=%s", retrieved ? retrieved : "null");
    cJSON_AddStringToObject(root, "status", status_buf);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}