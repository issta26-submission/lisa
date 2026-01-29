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
//<ID> 600
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
    cJSON *threshold = cJSON_CreateNumber(10.5);
    cJSON_AddItemToObject(config, "threshold", threshold);

    // step 2: Configure
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *threshold_ref = cJSON_GetObjectItem(config_ref, "threshold");
    double thr_val = cJSON_GetNumberValue(threshold_ref);
    cJSON *config_dup = cJSON_Duplicate(config_ref, 1);

    // step 3: Operate
    cJSON_AddItemToObject(root, "config_copy", config_dup);
    cJSON *config_tmp = cJSON_Duplicate(config_ref, 1);
    cJSON_Delete(config_tmp);

    // step 4: Validate & Cleanup
    cJSON *copied = cJSON_GetObjectItem(root, "config_copy");
    cJSON *copied_thr = cJSON_GetObjectItem(copied, "threshold");
    double copied_val = cJSON_GetNumberValue(copied_thr);
    (void)thr_val;
    (void)copied_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}