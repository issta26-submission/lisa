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
//<ID> 889
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *temp = cJSON_CreateNumber(23.5);
    cJSON *active = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "temperature", temp);
    cJSON_AddItemToObject(root, "active", active);

    // step 3: Operate & Validate
    cJSON *got_temp = cJSON_GetObjectItem(root, "temperature");
    double original_temp = cJSON_GetNumberValue(got_temp);
    double doubled_temp = cJSON_SetNumberHelper(got_temp, original_temp * 2.0);
    cJSON *doubled_item = cJSON_CreateNumber(doubled_temp);
    cJSON_AddItemToObjectCS(root, "temperature_doubled", doubled_item);
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_bool = cJSON_IsBool(got_active);
    cJSON *active_flag = cJSON_CreateNumber((double)active_is_bool);
    cJSON_AddItemToObject(root, "active_is_bool", active_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}