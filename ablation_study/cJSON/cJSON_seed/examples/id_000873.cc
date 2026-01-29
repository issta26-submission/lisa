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
//<ID> 873
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
    cJSON *threshold = cJSON_CreateNumber(42.0);
    const char *serial_id = "SN-00123";
    cJSON *serial = cJSON_CreateString(serial_id);

    // step 2: Configure
    cJSON_AddItemToObject(device, "serial", serial);
    cJSON_AddItemToObject(device, "threshold", threshold);
    cJSON_AddItemToObject(root, "device", device);

    // step 3: Operate & Validate
    cJSON *got_threshold = cJSON_GetObjectItem(device, "threshold");
    cJSON_bool threshold_is_number = cJSON_IsNumber(got_threshold);
    (void)threshold_is_number;
    cJSON *got_serial = cJSON_GetObjectItem(device, "serial");
    cJSON_bool serial_is_invalid = cJSON_IsInvalid(got_serial);
    (void)serial_is_invalid;
    cJSON *status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}