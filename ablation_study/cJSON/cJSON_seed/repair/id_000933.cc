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
//<ID> 933
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
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\": true}");
    cJSON_AddItemToObject(root, "raw_data", raw);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement, "replaced", 1.0);
    cJSON_AddStringToObject(root, "name", "example_node");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    cJSON *target = cJSON_GetObjectItemCaseSensitive(root, "raw_data");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, target, replacement);
    (void)replaced;
    cJSON *queried = cJSON_GetObjectItemCaseSensitive(root, "raw_data");
    (void)queried;
    char *printed_unformatted = cJSON_PrintUnformatted(root);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)ok;
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(snapshot);
    cJSON_Delete(dup);
    cJSON_free(printed_unformatted);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}