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
//<ID> 988
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *device_name = cJSON_CreateString("device-X");
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "name", device_name);

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddFalseToObject(root, "enabled");
    cJSON *override_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "override", override_true);

    // step 3: Operate & Validate
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    int is_false_flag = cJSON_IsFalse(got_enabled);
    double base_threshold = 200.0;
    double computed_threshold = base_threshold - (double)is_false_flag * 75.0;
    cJSON *threshold = cJSON_CreateNumber(computed_threshold);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}