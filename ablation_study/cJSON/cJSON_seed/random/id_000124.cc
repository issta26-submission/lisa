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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *s_sensor = (cJSON *)0;
    cJSON *s_status = (cJSON *)0;
    cJSON *readings_arr = (cJSON *)0;
    char *json_text = (char *)0;
    double count_val = 0.0;
    float readings_init[3] = { 1.1f, 2.2f, 3.3f };

    // step 2: Initialize
    root = cJSON_CreateObject();
    s_sensor = cJSON_CreateString("sensor-01");
    cJSON_AddItemToObject(root, "id", s_sensor);

    // step 3: Configure
    s_status = cJSON_AddStringToObject(root, "status", "ok");
    readings_arr = cJSON_CreateFloatArray(readings_init, 3);
    cJSON_AddItemToObject(root, "readings", readings_arr);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 4: Operate
    json_text = cJSON_Print(root);
    cJSON_free(json_text);
    count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    cJSON_AddNumberToObject(root, "validated_count", count_val);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}