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
//<ID> 753
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(payload, "maybe_null", maybe_null);
    cJSON *embedded_raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(payload, "embedded_raw", embedded_raw);
    cJSON *subobj = cJSON_CreateObject();
    cJSON_AddItemToObject(payload, "sub", subobj);
    cJSON *inner_raw = cJSON_CreateRaw("[1,2,3]");
    cJSON_AddItemToObject(subobj, "inner_raw", inner_raw);

    // step 3: Operate and Validate
    cJSON *found_payload = cJSON_GetObjectItem(root, "payload");
    cJSON *found_raw = cJSON_GetObjectItem(found_payload, "embedded_raw");
    cJSON *raw_dup = cJSON_Duplicate(found_raw, 1);
    cJSON_AddItemToObject(meta, "embedded_raw_copy", raw_dup);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}