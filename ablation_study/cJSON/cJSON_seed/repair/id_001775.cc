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
//<ID> 1775
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
    cJSON *id = cJSON_CreateString("unit-42");
    cJSON_AddItemToObject(device, "id", id);
    cJSON *metric = cJSON_CreateObject();
    cJSON_AddItemToObject(device, "metric", metric);
    cJSON *temp = cJSON_CreateNumber(36.6);
    cJSON_AddItemToObject(metric, "temp", temp);
    cJSON *state = cJSON_CreateString("running");
    cJSON_AddItemToObject(metric, "state", state);

    // step 2: Configure
    cJSON *metric_copy = cJSON_Duplicate(metric, 1);
    cJSON_bool identical = cJSON_Compare(metric, metric_copy, 1);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 3: Operate and Validate
    cJSON *got_device = cJSON_GetObjectItem(root, "device");
    cJSON *got_id = cJSON_GetObjectItem(got_device, "id");
    cJSON *got_metric = cJSON_GetObjectItem(got_device, "metric");
    cJSON *got_temp = cJSON_GetObjectItem(got_metric, "temp");
    cJSON *got_state = cJSON_GetObjectItem(got_metric, "state");
    cJSON_bool id_is_string = cJSON_IsString(got_id);
    char *id_str = cJSON_GetStringValue(got_id);
    double temp_val = cJSON_GetNumberValue(got_temp);
    char *state_str = cJSON_GetStringValue(got_state);
    buffer[0] = printed[0];
    buffer[1] = id_is_string ? id_str[0] : 'X';
    buffer[2] = (char)('0' + ((int)temp_val % 10));
    buffer[3] = state_str[0];
    buffer[4] = (char)('0' + (int)identical);
    (void)buffer;
    (void)id_str;
    (void)state_str;
    (void)temp_val;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(metric_copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}