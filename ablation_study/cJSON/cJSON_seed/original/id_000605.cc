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
//<ID> 605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 3.14);
    cJSON *threshold_item = cJSON_GetObjectItem(config, "threshold");
    double thr_value = cJSON_GetNumberValue(threshold_item);

    // step 3: Operate
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", dup_config);
    double adjusted_value = thr_value + 2.0;
    cJSON *adjusted_item = cJSON_CreateNumber(adjusted_value);
    cJSON_AddItemToObject(root, "adjusted", adjusted_item);
    cJSON *root_copy = cJSON_Duplicate(root, 1);

    // step 4: Validate & Cleanup
    cJSON *copied_config = cJSON_GetObjectItem(root_copy, "config_copy");
    cJSON *copied_threshold = cJSON_GetObjectItem(copied_config, "threshold");
    double copied_thr_value = cJSON_GetNumberValue(copied_threshold);
    (void)copied_thr_value;
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}