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
//<ID> 976
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "payload", payload);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "version", 3.1415);
    cJSON_AddStringToObject(config, "name", "example_name");
    cJSON_AddStringToObject(payload, "data", "hello");
    cJSON_AddNumberToObject(payload, "count", 2.0);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "payload");
    cJSON_AddStringToObject(detached, "status", "detached");
    cJSON_AddNumberToObject(detached, "removed_at", 42.0);
    cJSON_AddItemToObjectCS(root, "payload", detached);
    char buffer[256];
    memset(buffer, 0, 256);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 256, 1);
    cJSON *cfg_lookup = cJSON_GetObjectItem(root, "config");
    cJSON *ver_item = cJSON_GetObjectItem(cfg_lookup, "version");
    double ver = cJSON_GetNumberValue(ver_item);
    (void)printed;
    (void)ver;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}