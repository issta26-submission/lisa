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
//<ID> 790
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *device = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("sensor-A");
    cJSON *model_item = cJSON_CreateString("X100");
    cJSON_AddItemToObject(device, "name", name_item);
    cJSON_AddItemToObject(device, "model", model_item);
    cJSON_AddItemToObject(root, "device", device);
    cJSON *status_item = cJSON_CreateString("operational");
    cJSON_AddItemToObject(root, "status", status_item);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_device = cJSON_GetObjectItemCaseSensitive(parsed, "device");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_device, "name");
    char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON *copied_name = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(parsed, "copied_name", copied_name);
    char *verified = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "copied_name"));
    (void)verified;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}