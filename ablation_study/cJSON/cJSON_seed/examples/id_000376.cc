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
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("greeting");
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "label", label);
    cJSON_AddItemToObject(child, "value", num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "maybe_missing", maybe_missing);

    // step 2: Configure
    cJSON *dup_label = cJSON_Duplicate(label, 1);
    cJSON_AddItemToObject(root, "dup_label", dup_label);
    char *meta_buf = (char *)cJSON_malloc(64);
    memset(meta_buf, 0, 64);
    sprintf(meta_buf, "value=%.5f", cJSON_GetNumberValue(num));
    cJSON *meta = cJSON_CreateString(meta_buf);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_free(meta_buf);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(maybe_missing);
    char status_buf[32];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%d", (int)was_null);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    double retrieved = cJSON_GetNumberValue(num);
    cJSON *increment = cJSON_CreateNumber(retrieved + 1.0);
    cJSON_AddItemToObject(root, "incremented", increment);
    cJSON *child_copy = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", child_copy);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}