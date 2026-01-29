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
//<ID> 752
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
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "payload", payload);

    // step 2: Configure
    cJSON *raw_blob = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(payload, "raw_blob", raw_blob);
    cJSON *opt_null = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional_value", opt_null);
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "version", version);

    // step 3: Operate and Validate
    cJSON *payload_ref = cJSON_GetObjectItem(root, "payload");
    cJSON *raw_ref = cJSON_GetObjectItem(payload_ref, "raw_blob");
    char *raw_snapshot = cJSON_PrintUnformatted(raw_ref);
    cJSON_free(raw_snapshot);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}