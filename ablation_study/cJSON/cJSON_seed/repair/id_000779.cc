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
//<ID> 779
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *header = cJSON_AddObjectToObject(root, "header");
    cJSON *body = cJSON_AddObjectToObject(root, "body");

    // step 2: Configure
    cJSON_AddNumberToObject(header, "version", 1.2);
    cJSON_AddNumberToObject(header, "count", 3.0);
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(body, "payload", payload);
    cJSON_AddNumberToObject(payload, "score", 99.5);

    // step 3: Operate and Validate
    cJSON_bool header_is_obj = cJSON_IsObject(header);
    cJSON_bool body_is_obj = cJSON_IsObject(body);
    cJSON *detached = cJSON_DetachItemFromObject(body, "payload");
    cJSON_AddItemToObject(root, "detached_payload", detached);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}