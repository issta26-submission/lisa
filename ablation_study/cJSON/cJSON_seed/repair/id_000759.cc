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
//<ID> 759
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
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "payload", payload);

    // step 2: Configure
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(payload, "optional", optional);
    const char *raw_content = "{\"note\":\"raw\",\"n\":10}";
    cJSON *raw_blob = cJSON_CreateRaw(raw_content);
    cJSON_AddItemToObject(payload, "raw_blob", raw_blob);

    // step 3: Operate and Validate
    cJSON *payload_ref = cJSON_GetObjectItem(root, "payload");
    cJSON *optional_ref = cJSON_GetObjectItem(payload_ref, "optional");
    cJSON *raw_ref = cJSON_GetObjectItem(payload_ref, "raw_blob");
    const char *raw_snapshot = cJSON_GetStringValue(raw_ref);
    cJSON_AddStringToObject(meta, "raw_snapshot", raw_snapshot);
    cJSON *marker = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "marker", marker);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}