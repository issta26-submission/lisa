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
//<ID> 1020
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *device = NULL;
    cJSON *name = NULL;
    cJSON *online = NULL;
    cJSON *logs = NULL;
    cJSON *num = NULL;
    cJSON *detached = NULL;
    cJSON *dup = NULL;
    cJSON *got_online = NULL;
    char *printed = NULL;
    cJSON_bool is_online = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    device = cJSON_CreateObject();
    name = cJSON_CreateString("sensorX");
    cJSON_AddItemToObject(device, "name", name);
    online = cJSON_CreateTrue();
    cJSON_AddItemToObject(device, "online", online);
    cJSON_AddItemToObject(root, "device", device);
    logs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "logs", logs);
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(logs, num);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObject(root, "device");
    dup = cJSON_Duplicate(detached, 1);
    cJSON_AddItemToArray(logs, dup);
    got_online = cJSON_GetObjectItem(dup, "online");
    is_online = cJSON_IsTrue(got_online);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)is_online;
    return 66;
}