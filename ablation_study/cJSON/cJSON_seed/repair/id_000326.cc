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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *temp_item = cJSON_CreateNumber(23.5);
    cJSON *pressure_item = cJSON_CreateNumber(101.3);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON_bool added_metrics = cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_bool added_temp = cJSON_AddItemToObject(metrics, "temperature", temp_item);
    cJSON_bool added_pressure = cJSON_AddItemToObject(metrics, "pressure", pressure_item);
    cJSON *stable_item = cJSON_AddBoolToObject(metrics, "stable", 0);

    // step 3: Operate and Validate
    cJSON *got_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *got_temp = cJSON_GetObjectItem(got_metrics, "temperature");
    double temp_value = cJSON_GetNumberValue(got_temp);
    cJSON *got_pressure = cJSON_GetObjectItem(got_metrics, "pressure");
    double pressure_value = cJSON_GetNumberValue(got_pressure);
    double total = temp_value + pressure_value;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_bool added_sum = cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}