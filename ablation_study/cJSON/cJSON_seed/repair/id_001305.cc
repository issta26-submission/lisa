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
//<ID> 1305
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
    cJSON *name_item = cJSON_CreateString("Tester");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "enabled", enabled);

    // step 2: Configure
    cJSON *version_item = cJSON_AddStringToObject(root, "version", "1.0");
    char *name_val = (char *)cJSON_GetStringValue(name_item);
    char *out = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled);
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled);
    char *updated = cJSON_SetValuestring(name_item, "TesterX");
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = name_val ? name_val[0] : 0;
    ((char *)scratch)[2] = updated ? updated[0] : 0;
    ((char *)scratch)[3] = (char)(enabled_is_bool + enabled_is_true);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}