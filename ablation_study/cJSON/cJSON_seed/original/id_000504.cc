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
//<ID> 504
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    cJSON *enabled = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_bool enabled_true = cJSON_IsTrue(enabled);

    // step 3: Operate
    char *greeting_str = cJSON_GetStringValue(greeting);
    cJSON *payload = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON_bool payload_is_raw = cJSON_IsRaw(payload);
    int buf_len = 16 + (int)(greeting_str != NULL) + (int)enabled_true + (int)payload_is_raw;
    void *buffer = cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)greeting_str;
    (void)enabled_true;
    (void)payload_is_raw;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}