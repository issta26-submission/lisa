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
//<ID> 382
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
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    cJSON_AddNumberToObject(metrics, "temperature", 23.5);
    cJSON_AddNumberToObject(metrics, "pressure", 1013.25);
    cJSON_AddBoolToObject(metrics, "sensor_ok", 1);

    // step 3: Operate and Validate
    cJSON *temp_item = cJSON_GetObjectItemCaseSensitive(metrics, "temperature");
    cJSON *pressure_item = cJSON_GetObjectItemCaseSensitive(metrics, "pressure");
    cJSON *ok_item = cJSON_GetObjectItemCaseSensitive(metrics, "sensor_ok");
    double temp_val = cJSON_GetNumberValue(temp_item);
    double pressure_val = cJSON_GetNumberValue(pressure_item);
    double ok_val = (double)cJSON_IsTrue(ok_item);
    double total = temp_val + pressure_val + ok_val;
    double average = total / 3.0;
    cJSON_AddNumberToObject(root, "total", total);
    cJSON_AddNumberToObject(root, "average", average);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}