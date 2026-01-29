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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *threshold = cJSON_CreateNumber(100.5);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "active", active);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *scale = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "scale", scale);

    // step 3: Operate and Validate
    char *name_val = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *got_active = cJSON_GetObjectItem(config, "active");
    cJSON_bool is_active = cJSON_IsTrue(got_active);
    cJSON *active_num = cJSON_CreateNumber((double)is_active);
    cJSON_AddItemToObject(root, "active_num", active_num);
    cJSON *got_threshold = cJSON_GetObjectItem(config, "threshold");
    double thr = cJSON_GetNumberValue(got_threshold);
    double scale_val = cJSON_GetNumberValue(scale);
    cJSON *scaled = cJSON_CreateNumber(thr * scale_val);
    cJSON_AddItemToObject(root, "scaled_threshold", scaled);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}