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
//<ID> 2265
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
    cJSON_AddItemToObject(root, "device", device);
    cJSON *name = cJSON_CreateString("sensor-x");
    cJSON_AddItemToObject(device, "name", name);
    cJSON *val = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(device, "value", val);

    // step 2: Configure
    cJSON *device_dup = cJSON_Duplicate(device, 1);
    cJSON_AddItemToObject(root, "device_copy", device_dup);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver ? ver : "");
    cJSON_AddItemToObject(root, "lib_version", ver_item);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddItemToObject(replacement, "replaced", cJSON_CreateTrue());
    cJSON_ReplaceItemViaPointer(root, device_dup, replacement);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = ver ? ver[0] : '\0';
    buffer[2] = (char)((int)cJSON_GetNumberValue(val) & 0xFF);
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}