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
//<ID> 1304
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "DeviceX");
    cJSON *status_item = cJSON_AddStringToObject(meta, "status", "idle");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "enabled", enabled);

    // step 2: Configure
    cJSON_bool is_bool = cJSON_IsBool(enabled);
    cJSON_bool is_true = cJSON_IsTrue(enabled);

    // step 3: Operate and Validate
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (int)is_bool);
    ((char *)scratch)[1] = (char)('0' + (int)is_true);
    const char *name_val = cJSON_GetStringValue(name_item);
    ((char *)scratch)[2] = name_val[0];
    char *out = cJSON_PrintUnformatted(root);
    ((char *)scratch)[3] = out[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}