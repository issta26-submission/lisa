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
//<ID> 880
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
    cJSON *temp = cJSON_CreateNumber(25.0);
    cJSON *active = cJSON_CreateBool((cJSON_bool)1);
    cJSON *count = cJSON_CreateNumber(7.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddItemToObjectCS(metrics, "temperature", temp);
    cJSON_AddItemToObject(metrics, "active", active);
    cJSON_AddItemToObjectCS(root, "count", count);

    // step 3: Operate & Validate
    cJSON *got_temp = cJSON_GetObjectItem(metrics, "temperature");
    double temp_val = cJSON_GetNumberValue(got_temp);
    cJSON *double_temp = cJSON_CreateNumber(temp_val * 2.0);
    cJSON_AddItemToObject(metrics, "double_temperature", double_temp);
    cJSON_bool temp_is_number = cJSON_IsNumber(got_temp);
    cJSON *temp_is_number_flag = cJSON_CreateNumber((double)temp_is_number);
    cJSON_AddItemToObject(root, "temperature_is_number", temp_is_number_flag);
    cJSON *got_active = cJSON_GetObjectItem(metrics, "active");
    cJSON_bool active_is_bool = cJSON_IsBool(got_active);
    cJSON *active_is_bool_flag = cJSON_CreateNumber((double)active_is_bool);
    cJSON_AddItemToObjectCS(root, "active_is_bool", active_is_bool_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}