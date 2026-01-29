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
//<ID> 887
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
    cJSON *temp = cJSON_CreateNumber(23.5);
    cJSON *pressure = cJSON_CreateNumber(101.3);
    cJSON *active = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddItemToObjectCS(root, "active", active);
    cJSON_AddItemToObject(metrics, "temperature", temp);
    cJSON_AddItemToObjectCS(metrics, "pressure", pressure);

    // step 3: Operate & Validate
    cJSON *got_temp = cJSON_GetObjectItem(metrics, "temperature");
    cJSON_bool temp_is_num = cJSON_IsNumber(got_temp);
    cJSON *temp_valid = cJSON_CreateNumber((double)temp_is_num);
    cJSON_AddItemToObject(root, "temp_is_number", temp_valid);
    double tval = cJSON_GetNumberValue(got_temp);
    cJSON *t_half = cJSON_CreateNumber(tval / 2.0);
    cJSON_AddItemToObjectCS(metrics, "t_half", t_half);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}