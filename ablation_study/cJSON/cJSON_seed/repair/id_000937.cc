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
//<ID> 937
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
    cJSON_AddStringToObject(config, "name", "sensorA");
    cJSON_AddNumberToObject(config, "threshold", 3.14);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(config, "raw_payload", raw);
    cJSON *old_mode = cJSON_CreateString("temp");
    cJSON_AddItemToObject(config, "mode", old_mode);
    cJSON *new_mode = cJSON_CreateString("permanent");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(config, old_mode, new_mode);
    (void)replaced;

    // step 3: Operate and Validate
    cJSON *queried_config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *queried_mode = cJSON_GetObjectItemCaseSensitive(queried_config, "mode");
    const char *mode_value = cJSON_GetStringValue(queried_mode);
    (void)mode_value;
    char *snapshot = cJSON_PrintUnformatted(root);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}