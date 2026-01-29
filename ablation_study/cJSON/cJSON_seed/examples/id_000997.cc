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
//<ID> 997
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *devices = cJSON_CreateArray();
    cJSON *device = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("sensor-X");
    cJSON *id = cJSON_CreateNumber(42.0);
    int nums[3];
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    cJSON *int_arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObjectCS(device, "name", name);
    cJSON_AddItemToObjectCS(device, "id", id);
    cJSON_AddItemToObjectCS(device, "values", int_arr);
    cJSON_AddItemToArray(devices, device);
    cJSON_AddItemToObjectCS(root, "devices", devices);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(meta, "version", cJSON_CreateString("v1.0"));

    // step 2: Operate
    int device_count = cJSON_GetArraySize(devices);
    cJSON *first_device = cJSON_GetArrayItem(devices, 0);
    cJSON *got_name = cJSON_GetObjectItem(first_device, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObjectCS(first_device, "name_copy", name_copy);
    cJSON_bool has_id = cJSON_HasObjectItem(first_device, "id");
    cJSON_AddItemToObjectCS(root, "device_has_id", cJSON_CreateNumber((double)has_id));
    cJSON_AddItemToObjectCS(meta, "device_count", cJSON_CreateNumber((double)device_count));

    // step 3: Validate
    cJSON *got_name_copy = cJSON_GetObjectItem(first_device, "name_copy");
    char *copy_str = cJSON_GetStringValue(got_name_copy);
    (void)copy_str;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}