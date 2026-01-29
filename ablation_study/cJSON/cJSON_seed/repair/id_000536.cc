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
//<ID> 536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag_true);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "deprecated", flag_false);
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddNumberToObject(settings, "threshold", 3.14);

    // step 2: Configure
    const cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    double scaled_val = count_val * 2.5;
    cJSON *scaled_item = cJSON_CreateNumber(scaled_val);
    cJSON_AddItemToObject(root, "scaled_count", scaled_item);

    // step 3: Operate
    cJSON *settings_dup = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_dup);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *got_threshold = cJSON_GetObjectItem(settings_dup, "threshold");
    double thr_val = cJSON_GetNumberValue(got_threshold);
    (void)thr_val;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}