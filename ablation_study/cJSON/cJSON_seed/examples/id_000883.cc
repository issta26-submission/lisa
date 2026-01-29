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
//<ID> 883
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *temp = cJSON_CreateNumber(21.5);
    cJSON *active = cJSON_CreateBool(1);
    cJSON *meta = cJSON_CreateObject();
    cJSON *sensor_id = cJSON_CreateNumber(42);

    // step 2: Configure
    cJSON_AddItemToObject(root, "temperature", temp);
    cJSON_AddItemToObjectCS(root, "active", active);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObjectCS(meta, "id", sensor_id);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *got_temp = cJSON_GetObjectItem(root, "temperature");
    double temp_val = cJSON_GetNumberValue(got_temp);
    cJSON *is_high = cJSON_CreateBool((cJSON_bool)(temp_val > 25.0));
    cJSON_AddItemToObject(root, "is_high", is_high);
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_true = cJSON_IsTrue(got_active);
    cJSON *active_flag = cJSON_CreateNumber((double)active_true);
    cJSON_AddItemToObjectCS(root, "active_flag", active_flag);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}