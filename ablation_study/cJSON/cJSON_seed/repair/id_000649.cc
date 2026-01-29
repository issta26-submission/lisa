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
//<ID> 649
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
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON *status = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", status);
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON_AddStringToObject(payload, "initial", "ok");

    // step 2: Configure
    cJSON_AddStringToObject(payload, "type", "demo");
    cJSON_AddNumberToObject(payload, "count", 3.0);
    cJSON *greeting = cJSON_CreateString("world");
    cJSON_AddItemToObject(payload, "greeting", greeting);
    cJSON *version = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(meta, "version", version);

    // step 3: Operate and Validate
    const char *greet = cJSON_GetStringValue(cJSON_GetObjectItem(payload, "greeting"));
    cJSON_AddStringToObject(root, "greet_readback", greet);
    cJSON_bool status_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "status"));
    cJSON_AddBoolToObject(root, "status_is_false", status_is_false);
    cJSON *payload_copy = cJSON_Duplicate(payload, 1);
    cJSON_AddItemToObject(root, "payload_copy", payload_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    (void)printed;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}