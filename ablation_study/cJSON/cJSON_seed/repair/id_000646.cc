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
//<ID> 646
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
    cJSON *name = cJSON_AddStringToObject(meta, "name", "example");
    cJSON *status_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "alive", status_false);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);

    // step 2: Configure
    char *name_val = cJSON_GetStringValue(name);
    cJSON_AddStringToObject(root, "name_copy", name_val);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(info, "snapshot_before", snapshot);
    cJSON_free(snapshot);

    // step 3: Operate and Validate
    cJSON_bool alive_is_false = cJSON_IsFalse(status_false);
    cJSON_AddNumberToObject(root, "alive_is_false", (double)alive_is_false);
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", ver);

    // step 4: Cleanup
    char *final_out = cJSON_PrintUnformatted(root);
    cJSON_free(final_out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}