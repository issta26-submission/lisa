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
//<ID> 1009
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *dup = NULL;
    cJSON *name_item = NULL;
    cJSON *online_item = NULL;
    cJSON *config_item = NULL;
    cJSON *got_online = NULL;
    char *printed = NULL;
    const char *device_name = "sensorX";
    const char *name_val = NULL;
    cJSON_bool online_val = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString(device_name);
    cJSON_AddItemToObject(root, "name", name_item);
    online_item = cJSON_AddTrueToObject(root, "online");
    config_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "config", config_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    got_online = cJSON_GetObjectItem(dup, "online");
    online_val = cJSON_IsTrue(got_online);
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}