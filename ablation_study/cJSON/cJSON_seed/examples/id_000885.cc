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
//<ID> 885
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "value", num);
    cJSON_AddItemToObject(root, "active", flag);
    cJSON_AddItemToObject(root, "sensor", sensor);
    cJSON *sensor_id = cJSON_CreateString("id-123");
    cJSON_AddItemToObjectCS(sensor, "id", sensor_id);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *gotValue = cJSON_GetObjectItem(root, "value");
    double v = cJSON_GetNumberValue(gotValue);
    cJSON *value_plus_one = cJSON_CreateNumber(v + 1.0);
    cJSON_AddItemToObject(root, "value_plus_one", value_plus_one);
    cJSON *gotActive = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_bool = cJSON_IsBool(gotActive);
    cJSON *active_is_bool = cJSON_CreateNumber((double)is_bool);
    cJSON_AddItemToObjectCS(root, "active_is_bool", active_is_bool);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}