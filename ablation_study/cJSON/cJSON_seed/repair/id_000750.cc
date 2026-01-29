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
//<ID> 750
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
    cJSON_AddItemToObject(root, "payload", payload);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(payload, "raw_section", raw);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(payload, "maybe_missing", maybe_missing);

    // step 3: Operate and Validate
    cJSON *payload_ref = cJSON_GetObjectItem(root, "payload");
    cJSON *raw_ref = cJSON_GetObjectItem(payload_ref, "raw_section");
    const char *raw_str = cJSON_GetStringValue(raw_ref);
    cJSON *copied_raw = cJSON_CreateRaw(raw_str ? raw_str : "{}");
    cJSON_AddItemToObject(root, "copied_raw", copied_raw);
    cJSON *maybe_ref = cJSON_GetObjectItem(payload_ref, "maybe_missing");
    cJSON_bool is_null = cJSON_IsNull(maybe_ref);
    cJSON_AddNumberToObject(root, "maybe_missing_is_null", (double)is_null);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}