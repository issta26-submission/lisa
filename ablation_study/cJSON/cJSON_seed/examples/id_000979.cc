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
//<ID> 979
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
    cJSON *device = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON *temp = cJSON_CreateNumber(21.75);
    cJSON *id = cJSON_CreateNumber(7.0);
    cJSON *status = cJSON_CreateString("active");

    // step 2: Configure
    cJSON_AddItemToObject(device, "name", name);
    cJSON_AddItemToObject(device, "temp", temp);
    cJSON_AddItemToObject(device, "id", id);
    cJSON_AddItemToArray(devices, device);
    cJSON_AddItemToObject(root, "devices", devices);
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate & Validate
    cJSON *first_device = cJSON_GetArrayItem(devices, 0);
    cJSON *got_temp = cJSON_GetObjectItem(first_device, "temp");
    double current_temp = cJSON_GetNumberValue(got_temp);
    double updated_temp = cJSON_SetNumberHelper(got_temp, current_temp + 1.25);
    (void)updated_temp;
    cJSON *got_name = cJSON_GetObjectItem(first_device, "name");
    char *current_name = cJSON_GetStringValue(got_name);
    cJSON *last_seen = cJSON_CreateString(current_name);
    cJSON_AddItemToObject(first_device, "last_seen_name", last_seen);
    cJSON *extra_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(devices, extra_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}