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
//<ID> 437
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
    cJSON *name_item = cJSON_CreateString("cJSON_test_v1");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *info = cJSON_AddObjectToObject(root, "info");
    cJSON_AddItemToObject(info, "status", cJSON_CreateString("starter"));

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(info, "active", cJSON_CreateTrue());

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "name"));
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s|snapshot_len=%zu", name_val ? name_val : "null", snapshot ? (size_t)strlen(snapshot) : 0);
    cJSON_AddStringToObject(root, "status", status_buf);
    cJSON_AddStringToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}