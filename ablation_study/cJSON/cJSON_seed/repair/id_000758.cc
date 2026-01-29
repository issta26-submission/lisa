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
//<ID> 758
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

    // step 2: Configure
    cJSON *raw_blob = cJSON_CreateRaw("{\"raw_key\":true}");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "raw_blob", raw_blob);
    cJSON_AddItemToObject(config, "optional", null_item);

    // step 3: Operate and Validate
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *raw_ref = cJSON_GetObjectItem(config_ref, "raw_blob");
    cJSON *null_ref = cJSON_GetObjectItem(config_ref, "optional");
    cJSON *summary = cJSON_CreateObject();
    cJSON *raw_dup = cJSON_Duplicate(raw_ref, 1);
    cJSON *null_dup = cJSON_Duplicate(null_ref, 1);
    cJSON_AddItemToObject(summary, "copied_raw", raw_dup);
    cJSON_AddItemToObject(summary, "copied_optional", null_dup);
    cJSON_AddItemToObject(root, "summary", summary);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}