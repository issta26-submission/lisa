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
//<ID> 879
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *temp = cJSON_CreateNumber(24.5);
    cJSON *status = cJSON_CreateString("ok");
    cJSON *meta = cJSON_CreateObject();
    cJSON *sensor_id = cJSON_CreateString("sensor-A");

    // step 2: Configure
    cJSON_AddItemToObject(root, "temperature", temp);
    cJSON_AddItemToObject(root, "status", status);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "id", sensor_id);

    // step 3: Operate & Validate
    cJSON *got_temp = cJSON_GetObjectItem(root, "temperature");
    cJSON_bool temp_is_number = cJSON_IsNumber(got_temp);
    cJSON *temp_check_num = cJSON_CreateNumber((double)temp_is_number);
    cJSON_AddItemToObject(root, "temp_is_number", temp_check_num);
    cJSON *got_status = cJSON_GetObjectItem(root, "status");
    cJSON_bool status_invalid = cJSON_IsInvalid(got_status);
    cJSON *status_invalid_flag = cJSON_CreateNumber((double)status_invalid);
    cJSON_AddItemToObject(root, "status_invalid", status_invalid_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}