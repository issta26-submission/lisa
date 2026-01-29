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
//<ID> 740
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "threshold", 3.1415);
    cJSON_AddNumberToObject(root, "global_count", 7.0);

    // step 2: Configure
    cJSON *threshold = cJSON_GetObjectItem(config, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold);
    cJSON *global_count = cJSON_GetObjectItem(root, "global_count");
    double global_count_value = cJSON_GetNumberValue(global_count);

    // step 3: Operate and Validate
    cJSON *detached_config = cJSON_DetachItemFromObject(root, "config");
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_bool detached_is_object = cJSON_IsObject(detached_config);

    // step 4: Cleanup
    (void)threshold_value;
    (void)global_count_value;
    (void)root_is_object;
    (void)detached_is_object;
    cJSON_Delete(detached_config);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}