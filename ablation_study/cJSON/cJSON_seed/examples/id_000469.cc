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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "name", "example");

    // step 2: Configure
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddStringToObject(meta, "owner", "tester");

    // step 3: Operate & Validate
    cJSON *meta_retrieved = cJSON_GetObjectItem(root, "meta");
    cJSON *status_item = cJSON_GetObjectItem(meta_retrieved, "status");
    char *status_value = cJSON_GetStringValue(status_item);
    char updated_buf[128];
    memset(updated_buf, 0, sizeof(updated_buf));
    sprintf(updated_buf, "%s-updated", status_value);
    cJSON *replacement = cJSON_CreateString(updated_buf);
    cJSON_ReplaceItemViaPointer(meta_retrieved, status_item, replacement);
    char *new_status = cJSON_GetStringValue(cJSON_GetObjectItem(meta_retrieved, "status"));
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}