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
//<ID> 938
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
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "rawdata", raw);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced_raw");
    cJSON_AddNumberToObject(meta, "marker", 2026.0);

    // step 3: Operate and Validate
    cJSON *item_ptr = cJSON_GetObjectItemCaseSensitive(root, "rawdata");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, item_ptr, replacement);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced);
    cJSON *current = cJSON_GetObjectItemCaseSensitive(root, "rawdata");
    char *val = cJSON_GetStringValue(current);
    cJSON_AddStringToObject(root, "replaced_value", val);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(snapshot);
    cJSON_Delete(dup);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}