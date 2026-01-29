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
//<ID> 981
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
    cJSON *enabled = cJSON_CreateFalse();
    cJSON *name = cJSON_CreateString("device-42");

    // step 2: Configure
    cJSON_AddItemToObject(meta, "enabled", enabled);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_enabled = cJSON_GetObjectItem(got_meta, "enabled");
    cJSON_bool was_disabled = cJSON_IsFalse(got_enabled);
    cJSON *switched = cJSON_CreateBool(!was_disabled);
    cJSON *explicit_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(got_meta, "switched", switched);
    cJSON_AddItemToObject(got_meta, "explicit_true", explicit_true);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}