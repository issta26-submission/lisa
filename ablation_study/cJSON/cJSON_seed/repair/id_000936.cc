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
//<ID> 936
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

    // step 2: Configure
    cJSON_AddStringToObject(meta, "name", "test_node");
    cJSON *raw_item = cJSON_CreateRaw("{\"x\":1}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *found_raw = cJSON_GetObjectItemCaseSensitive(root, "raw");
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement, "x", 42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, found_raw, replacement);
    (void)replaced;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *dup_snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(dup_snapshot);
    cJSON_Delete(dup);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)ok;
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}