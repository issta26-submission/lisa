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
//<ID> 431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_value = "hello_world";
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *key_item = cJSON_CreateString(initial_value);
    cJSON_AddItemToObject(meta, "key", key_item);

    // step 2: Configure
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(info, "count", cJSON_CreateNumber(3));
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateBool(1));

    // step 3: Operate & Validate
    const char *extracted = cJSON_GetStringValue(key_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "extracted=%s|count=%d", extracted ? extracted : "null", 3);
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