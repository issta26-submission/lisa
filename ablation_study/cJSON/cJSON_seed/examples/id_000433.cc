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
//<ID> 433
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_name = "example_name";
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_CreateString(initial_name);
    cJSON *id_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON_AddItemToObject(meta, "id", id_item);

    // step 2: Configure
    cJSON *details = cJSON_AddObjectToObject(root, "details");
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(details, "active", flag_item);

    // step 3: Operate & Validate
    const char *retrieved_name = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "name"));
    double retrieved_id = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "id"));
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s|id=%.0f", retrieved_name ? retrieved_name : "null", retrieved_id);
    cJSON *status_item = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}