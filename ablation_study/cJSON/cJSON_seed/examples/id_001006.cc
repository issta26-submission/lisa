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
//<ID> 1006
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *devices = cJSON_CreateArray();
    cJSON *device1 = cJSON_CreateObject();
    cJSON *name1 = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(device1, "name", name1);
    cJSON_AddNumberToObject(device1, "value", 12.5);
    cJSON_AddItemToArray(devices, device1);
    cJSON *device2 = cJSON_CreateObject();
    cJSON *name2 = cJSON_CreateString("sensor-B");
    cJSON_AddItemToObject(device2, "name", name2);
    cJSON_AddNumberToObject(device2, "value", 42.0);
    cJSON_AddItemToArray(devices, device2);
    cJSON_AddItemToObject(root, "devices", devices);
    cJSON_AddNumberToObject(root, "total_devices", 2.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON *version = cJSON_CreateString("v1.0");
    cJSON_AddItemToObject(meta, "version", version);
    cJSON_AddNumberToObject(meta, "uptime", 3600.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *first_device = cJSON_GetArrayItem(devices, 0);
    cJSON *first_name = cJSON_GetObjectItem(first_device, "name");
    const char *fname = cJSON_GetStringValue(first_name);
    cJSON *fname_copy = cJSON_CreateString(fname);
    cJSON_AddItemToObject(root, "first_device_name_copy", fname_copy);
    cJSON *first_value_item = cJSON_GetObjectItem(first_device, "value");
    double first_value = cJSON_GetNumberValue(first_value_item);
    cJSON_AddNumberToObject(root, "first_value_double", first_value * 2.0);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}