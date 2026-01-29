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
//<ID> 427
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
    cJSON *stats = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "stats", stats);

    // step 2: Configure
    cJSON *threshold_item = cJSON_AddNumberToObject(config, "threshold", 3.14);
    cJSON *min_item = cJSON_AddNumberToObject(stats, "min", 1.0);
    cJSON *max_item = cJSON_AddNumberToObject(stats, "max", 9.0);
    cJSON *mean_item = cJSON_AddNumberToObject(stats, "mean", 5.0);

    // step 3: Operate
    cJSON_bool stats_is_object = cJSON_IsObject(stats);
    double threshold = cJSON_GetNumberValue(threshold_item);
    double min_v = cJSON_GetNumberValue(min_item);
    double max_v = cJSON_GetNumberValue(max_item);
    double mean_v = cJSON_GetNumberValue(mean_item);
    double range = max_v - min_v;
    double weighted = (mean_v * 0.7) + (threshold * 0.3) + (double)stats_is_object;
    double computed_value = weighted + range;

    // step 4: Validate and Cleanup
    cJSON *computed_item = cJSON_AddNumberToObject(root, "computed_value", computed_value);
    double retrieved = cJSON_GetNumberValue(computed_item);
    (void)retrieved;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}