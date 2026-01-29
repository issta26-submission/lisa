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
//<ID> 782
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"device\":{\"id\":42,\"name\":\"sensor\"},\"metrics\":[10,20,30]}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddNumberToObject(root, "uptime", 1024.5);

    // step 3: Operate and Validate
    cJSON *detached_device = cJSON_DetachItemFromObject(root, "device");
    cJSON *detached_container = cJSON_CreateObject();
    cJSON_AddItemToObject(detached_container, "device", detached_device);
    char *snapshot = cJSON_PrintUnformatted(detached_container);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(detached_container);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}