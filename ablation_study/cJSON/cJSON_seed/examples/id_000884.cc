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
//<ID> 884
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON *temp = cJSON_CreateNumber(24.75);
    cJSON *active = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "temperature", temp);
    cJSON_AddItemToObjectCS(root, "active", active);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *id = cJSON_CreateString("sensor-xyz");
    cJSON_AddItemToObjectCS(meta, "id", id);

    // step 3: Operate & Validate
    double tempval = cJSON_GetNumberValue(temp);
    int rounded_int = (int)(tempval + 0.5);
    cJSON *rounded = cJSON_CreateNumber((double)rounded_int);
    cJSON_AddItemToObjectCS(root, "rounded_temperature", rounded);
    cJSON *got_temp = cJSON_GetObjectItem(root, "temperature");
    cJSON_bool temp_is_number = cJSON_IsNumber(got_temp);
    cJSON *temp_is_number_item = cJSON_CreateBool(temp_is_number);
    cJSON_AddItemToObject(root, "temperature_is_number", temp_is_number_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}