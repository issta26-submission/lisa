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
//<ID> 870
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
    cJSON *name = cJSON_CreateString("device-42");
    cJSON *reading = cJSON_CreateNumber(123.456);

    // step 2: Configure
    cJSON_AddItemToObject(root, "device", device);
    cJSON_AddItemToObject(device, "name", name);
    cJSON_AddItemToObject(device, "reading", reading);

    // step 3: Operate & Validate
    cJSON *got_device = cJSON_GetObjectItem(root, "device");
    cJSON *got_reading = cJSON_GetObjectItem(got_device, "reading");
    cJSON_bool is_number = cJSON_IsNumber(got_reading);
    cJSON_bool is_invalid_name = cJSON_IsInvalid(name);
    cJSON *is_number_item = cJSON_CreateNumber((double)is_number);
    cJSON *is_invalid_item = cJSON_CreateBool(is_invalid_name);
    cJSON_AddItemToObject(root, "reading_is_number", is_number_item);
    cJSON_AddItemToObject(root, "name_is_invalid", is_invalid_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}