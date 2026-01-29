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
//<ID> 325
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
    cJSON_bool added_sensor = cJSON_AddItemToObject(root, "sensor", sensor);

    // step 2: Configure
    cJSON *base_num = cJSON_CreateNumber(21.0);
    cJSON_bool added_base = cJSON_AddItemToObject(sensor, "base", base_num);
    cJSON *enabled = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *got_base = cJSON_GetObjectItem(sensor, "base");
    double base_val = cJSON_GetNumberValue(got_base);
    double doubled = base_val * 2.0;
    cJSON *double_item = cJSON_CreateNumber(doubled);
    cJSON_bool added_double = cJSON_AddItemToObject(root, "double_base", double_item);
    cJSON *got_double = cJSON_GetObjectItem(root, "double_base");
    double double_check = cJSON_GetNumberValue(got_double);
    cJSON *large_flag = cJSON_AddBoolToObject(sensor, "large", double_check > 40.0);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}