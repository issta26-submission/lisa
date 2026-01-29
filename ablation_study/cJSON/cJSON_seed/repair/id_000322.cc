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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *sensor = cJSON_CreateObject();

    // step 2: Configure
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON *count_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON_AddItemToObject(root, "sensor", sensor);
    cJSON *sensor_temp = cJSON_CreateNumber(22.5);
    cJSON_AddItemToObject(sensor, "temp", sensor_temp);
    cJSON *active_item = cJSON_AddBoolToObject(sensor, "active", 1);

    // step 3: Operate and Validate
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    cJSON *got_sensor = cJSON_GetObjectItem(root, "sensor");
    cJSON *got_temp = cJSON_GetObjectItem(got_sensor, "temp");
    double temp_val = cJSON_GetNumberValue(got_temp);
    double combined = pi_val + temp_val;
    cJSON *sum_item = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "pi_plus_temp", sum_item);
    cJSON *got_sum = cJSON_GetObjectItem(root, "pi_plus_temp");
    double sum_val = cJSON_GetNumberValue(got_sum);
    (void)active_item;
    (void)count_item;
    (void)sum_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}