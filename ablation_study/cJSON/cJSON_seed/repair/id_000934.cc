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
//<ID> 934
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *orig = cJSON_CreateString("placeholder");
    cJSON_AddItemToObject(config, "data", orig);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"rawnum\":123}");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(config, orig, raw);
    (void)replaced;

    // step 3: Operate and Validate
    cJSON *queried_config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *queried_data = cJSON_GetObjectItemCaseSensitive(queried_config, "data");
    (void)queried_config;
    (void)queried_data;
    char *snapshot = cJSON_PrintUnformatted(root);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)ok;
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}