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
//<ID> 465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *count = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(meta, "count", count);

    // step 3: Operate & Validate
    cJSON *label_ptr = cJSON_GetObjectItem(root, "label");
    char *label_val = cJSON_GetStringValue(label_ptr);
    char status_before[128];
    memset(status_before, 0, sizeof(status_before));
    sprintf(status_before, "before=%s", label_val);
    cJSON_AddStringToObject(root, "status_before", status_before);
    cJSON *new_label = cJSON_CreateString("updated");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, label_ptr, new_label);
    (void)replaced;
    cJSON *post = cJSON_GetObjectItem(root, "label");
    char *post_val = cJSON_GetStringValue(post);
    char status_after[128];
    memset(status_after, 0, sizeof(status_after));
    sprintf(status_after, "after=%s", post_val);
    cJSON_AddStringToObject(root, "status_after", status_after);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}