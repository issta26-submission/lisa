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
//<ID> 608
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
    cJSON_AddNumberToObject(config, "threshold", 42.0);
    cJSON_AddStringToObject(config, "mode", "auto");

    // step 2: Configure
    cJSON *config_dup = cJSON_Duplicate(config, 1);
    cJSON *dup_threshold = cJSON_GetObjectItem(config_dup, "threshold");
    cJSON_SetNumberHelper(dup_threshold, cJSON_GetNumberValue(dup_threshold) + 10.0);

    // step 3: Operate
    cJSON_AddItemToObject(root, "config_copy", config_dup);
    cJSON *copied = cJSON_GetObjectItem(root, "config_copy");
    cJSON *copied_thresh = cJSON_GetObjectItem(copied, "threshold");
    double val = cJSON_GetNumberValue(copied_thresh);
    cJSON_AddNumberToObject(root, "copied_threshold", val);

    // step 4: Validate & Cleanup
    cJSON *final = cJSON_GetObjectItem(root, "copied_threshold");
    double final_val = cJSON_GetNumberValue(final);
    (void)final_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}