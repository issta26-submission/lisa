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
//<ID> 293
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
    cJSON *name_item = cJSON_CreateString("device-42");
    cJSON *count_item = cJSON_CreateNumber(7.0);
    cJSON *status_old = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "status", status_old);

    // step 3: Operate and Validate
    cJSON *meta_from_root = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_from_root);
    (void)meta_is_obj;
    cJSON *name_from_root = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_from_root);
    (void)name_str;
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    cJSON *status_new = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(meta_from_root, status_old, status_new);
    (void)replaced;
    cJSON *status_after = cJSON_GetObjectItem(meta_from_root, "status");
    cJSON_bool status_is_true = cJSON_IsTrue(status_after);
    (void)status_is_true;
    cJSON_AddNumberToObject(root, "count_scaled", count_val * 2.0);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}